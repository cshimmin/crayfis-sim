#!/usr/bin/env python

from PIL import Image
import numpy as np

def trans_linear(v,pmin=50e-6,pmax=0.01):
    ''' Linear transformation of energy->brightness.
      pmin: the minimum pixel energy (y-intercept)
      pmax: the max pixel energy (corresponding to saturation) '''
    return max(min(int(round((v-(pmin))*255./pmax)), 255),0)

def trans_quant(v, e_electron=3.6e-6, adc_lsb=25, adc_shift=4, pedestal=15):
    ''' Quantizing transformation of energy->brightness
      e_electron: avg. energy per electron
      adc_lsb: # of electrons per LSB on the ADC
      adc_shift: how many bits the ADC gets shifted (i.e. 10bit->8bit = shift of 2)
      pedestal: minimum number of electrons that can be read (this is subtracted off the electron count before ADC '''
    # quantize electrons
    v = int(v / e_electron)
    v -= pedestal
    # quantize ADC
    v = int(v / adc_lsb)
    # downsample ADC bits
    if adc_shift > 0:
        v = int(v / (2**adc_shift))
    return min(v, 255)

def trans_gamma(v):
    ''' Random ad-hoc transformation for comparison purposes. Somewhat analogous to adjusting the gamma level of an image '''
    return max(min(int(round((np.sqrt(v)-(3e-3))*255./0.1)), 255),0)

def get_bayer_color(x,y):
    ''' Return the color of the bayer filter at the given pixel coordinates.
    0=green, 1=red, 2=blue
    '''
    if (x+y)%2==0:
        return 1
    elif x%2==0:
        return 0
    else:
        return 2

def pixwrite_bw(img, x, y, v):
    ''' Write a grayscale pixel value to the image '''
    if x < 0 or y < 0: return
    if x > 499 or y > 499: return
    img.putpixel((x,y), v)

def pixwrite_bayer(img, x, y, v):
    ''' Write an RGB pixel value to the image based on the bayer filter color'''
    color = get_bayer_color(x,y)
    if color == 0:
        vc = (v,0,0)
    elif color == 1:
        vc = (0,v,0)
    elif color == 2:
        vc = (0,0,v)
    img.putpixel((x,y), vc)

def mkimg(t, entry, trans, mode="L", offset=None):
    ''' Make an image from a single event in the ROOT tree.
      t:      the input ROOT tree
      entry:  the entry number to write out
      trans:  the transformation function to use
      mode:   The image mode; "L" for grayscale, "bayer" for bayer-mosasic
      offset: (x,y) coordinate to shift the original image by
    '''
    if mode == "L":
        img = Image.new("L", (500,500))
        pixwrite = pixwrite_bw
    elif mode == "bayer":
        img = Image.new("RGB", (500,500))
        pixwrite = pixwrite_bayer
    else:
        raise RuntimeError("Unknown mode: %s" % mode)

    t.GetEntry(entry)

    for x,y,v,n1,n2,n3,n4,n5,n6,n7,n8 in zip(t.pix_x,t.pix_y,t.pix_val,t.pix_n1,t.pix_n2,t.pix_n3,t.pix_n4,t.pix_n5,t.pix_n6,t.pix_n7,t.pix_n8):
        if not offset is None:
            x += offset[0]
            y += offset[1]
        pixwrite(img, x, y, trans(v))
        pixwrite(img, x-1, y-1, trans(n1))
        pixwrite(img, x, y-1, trans(n2))
        pixwrite(img, x+1, y-1, trans(n3))

        pixwrite(img, x-1, y, trans(n4))
        pixwrite(img, x+1, y, trans(n5))

        pixwrite(img, x-1, y+1, trans(n6))
        pixwrite(img, x, y+1, trans(n7))
        pixwrite(img, x+1, y+1, trans(n8))
    
    return img

def random_composite(t, ntrack=20, trans=trans_linear):
    ''' Randomly select events from the given ROOT tree and overlay them onto a single image.
      t:      The input ROOT tree
      ntrack: The number of events to overlay
      trans:  The energy transformation to apply
    '''
    entries = np.random.randint(0,t.GetEntries()-1, ntrack)
    img_array = np.zeros_like(np.asarray(Image.new("L",(500,500))))
    for i in entries:
        offset = np.random.randint(-250,250,2)
        img_array += np.asarray(mkimg(t, i, trans, offset=offset))
    img = Image.fromarray(img_array)
    return img

if __name__ == "__main__":
    from argparse import ArgumentParser

    parser = ArgumentParser(description="Generate images from GEANT simluation")
    parser.add_argument("-N", type=int, default=20, help="The number of events to overlay.")
    parser.add_argument("input_file", help="The input ROOT file from GEANT simulation.")
    parser.add_argument("output_file", nargs="?", default="tracks.jpg", help="The output image file")
    args = parser.parse_args()

    import ROOT as r
    t = r.TChain("pixels")
    t.Add(args.input_file)

    img = random_composite(t, ntrack=args.N, trans=trans_quant)
    img.save(args.output_file, format='jpeg', quality=98)
    print "Saved to", args.output_file
