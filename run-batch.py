#!/usr/bin/env python

import os
import sys

import subprocess
import errno

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="submit jobs to the batch")

    parser.add_argument("--energy", default=1300, type=float, help="the energy in keV")
    parser.add_argument("--particle", default="gamma", type=str, help="the particle type")
    parser.add_argument("-N", default=1000, type=int, help="number of events to generate")
    parser.add_argument("--out", help="the output path")
    parser.add_argument("--seed1", default=1, type=int, help="seed 1")
    parser.add_argument("--seed2", default=2, type=int, help="seed 2")
    parser.add_argument("template", help="the template .mac file")

    args = parser.parse_args()

    if not "SLURM_JOBID" in os.environ:
        print "hey you're not on the batch"
        sys.exit(1)

    slurm_id = int(os.environ["SLURM_JOBID"])

    if args.out:
        try:
            os.mkdir(args.out)
        except OSError as e:
            if e.errno == errno.EEXIST:
                pass
            else:
                raise

    output_dir = "job_%d"%slurm_id
    if args.out:
        output_dir = os.path.join(args.out, output_dir)

    #if args.out.endswith(".root"):
    #    args.out = args.out[:-len(".root")]

    macro = str(open(args.template).read())

    outfile = os.path.join(output_dir, "events")

    macro = macro.format(
            energy=args.energy,
            particle=args.particle,
            ngen=args.N,
            seed1=args.seed1,
            seed2=args.seed2,
            outfile=outfile
            )
    macro = macro.replace("{{", "{").replace("}}", "}")

    try:
        os.mkdir(output_dir)
    except OSError as e:
        print "oops had problems making directory"
        sys.exit(1)

    macro_file_name = os.path.join(output_dir, "run.mac")
    macro_file = open(macro_file_name, 'w')
    macro_file.write(macro)
    macro_file.close()

    subprocess.call(['bin/TestEm1', macro_file_name])
