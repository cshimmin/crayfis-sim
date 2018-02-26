from utils import dict_product
import os
import os.path as osp

import numpy as np

__all__ = [
  'generate_configs'
]


def get_resourse(path, dir=True):
  here = osp.dirname(osp.abspath(__file__))

  resourse = osp.abspath(osp.join(here, path))
  assert osp.exists(resourse) and (osp.isdir if dir else osp.isfile)(resourse), \
    'resourse %(what) [%(where)] does not seem like a %s' % dict(
      where = resourse, what = 'directory' if dir else 'file'
    )

  return resourse

get_dir = lambda path: get_resourse(path, dir=True)
get_file = lambda path: get_resourse(path, dir=False)

def get_total_flux(path):
  import ROOT as r

  f = r.TFile(path)
  h = f.Get('particleEnergy')

  return np.sum([
    h.GetBinContent(i)
    for i in range(h.GetSize())
  ])


def generate_configs(output_dir, ngen=int(1.0e+5), pixDepth=1, pixWidth=1.5, npix=5000, spectra_path=None, jobs=1):
  hist_dir = get_dir('../data/background_spectra')
  hists = [
    osp.join(hist_dir, item)
    for item in os.listdir(hist_dir)
    if item.endswith('.root')
  ]

  particle_names = [
    osp.basename(hist)[:-len('.root')] for hist in hists
  ]

  fluxes = [ get_total_flux(hist) for hist in hists ]
  print("Total flux: %.3e" % np.sum(fluxes))
  priors = [ flux / np.sum(fluxes) for flux in fluxes ]
  print(
    "Priors:\n  %s" % '\n  '.join([
      "%s: %.3e" % (particle, prior)
      for particle, prior in zip(particle_names, priors)
    ])
  )

  assert len(hists), 'there is no data for cosmic background spectra!'

  if spectra_path is None:
    spectra_path = hist_dir

  runtime_hists = [
    osp.join(spectra_path, osp.basename(hist))
    for hist in hists
  ]

  configs = dict(
      beamEnergy=-1,
      particle_meta=[
        dict(
          output=osp.join(output_dir, '%s_%04d' % (osp.basename(hist)[:-len('.root')], job)),
          particle=particle_name,
          energyHisto=hist,
          job=job
        )
        for particle_name, hist in zip(particle_names, runtime_hists)
        for job in range(jobs)
      ],
      ngen=ngen,
      pixDepth=pixDepth,
      pixWidth=pixWidth,
      npix=npix,
  )

  return dict_product(configs)

def get_seeds(n, super_seed):
  import random
  random.seed(super_seed)

  numbers = []

  while len(set(numbers)) != n:
    numbers = [
      (random.randrange(int(1.0e+6)), random.randrange(int(1.0e+6)))
      for _ in range(n)
    ]

  return numbers

if __name__ == '__main__':
  import argparse
  import shutil as sh

  parser = argparse.ArgumentParser(description='Generate configs for cosmic background simulation.')
  parser.add_argument('ngen', type=str, help='number of particles of each type to shoot')
  parser.add_argument('-o', '--output', default='./', type=str, help='simulation output directory')
  parser.add_argument('-d', '--pixDepth', default=1.0, type=float, help='pixel depth')
  parser.add_argument('-w', '--pixWidth', default=1.5, type=float, help='pixel width')
  parser.add_argument('-n', '--npix', default=5000, type=float, help='linear size of the sensor')
  parser.add_argument('-r', '--runtime_spectra_path', default='data/background_spectra/', type=str, help='overrides path to spectra files')
  parser.add_argument('-s', '--super_seed', default=12345, type=int, help='seed to generate seeds')
  parser.add_argument('-j', '--jobs', default=1, type=int, help='split simulation between number of jobs (per particle type)')
  parser.add_argument('-c', '--configs_output', default='./configs/', type=str, help='output for the generated config files')

  args = parser.parse_args()

  with open(get_file('../data/config/run.mac.template'), 'r') as _f:
    from string import Template
    config = Template(_f.read())

  configs = list(generate_configs(
    output_dir = args.output,
    ngen = args.ngen,
    pixDepth = args.pixDepth,
    pixWidth = args.pixWidth,
    npix = args.npix,
    jobs=args.jobs,
    spectra_path=args.runtime_spectra_path
  ))

  try:
    os.makedirs(args.configs_output)
  except OSError:
    pass

  super_seed = args.super_seed

  for values, (seed1, seed2) in zip(configs, get_seeds(len(configs), super_seed=super_seed)):
    particle = values['particle']
    job = values['job']
    values['seed1'] = seed1
    values['seed2'] = seed2

    with open(osp.join(args.configs_output, '%s_%04d.mac' % (particle, job)), 'w') as f:
      f.write(config.substitute(values))
