#include "nwocg_run.h"
#include <math.h>

static struct
{
  double UD3;
  double S3;
  double G1;
  double S2;
  double UD1;
  double UD2;
  double S1;
  double in;
} nwocg;

void nwocg_generated_init()
{
  nwocg.UD2 = 0;
  nwocg.UD3 = 0;
  nwocg.UD1 = 0;
}

void nwocg_generated_step()
{
  nwocg.S1 = -nwocg.UD3 + nwocg.in;
  nwocg.S2 = nwocg.UD1 - nwocg.S1;
  nwocg.G1 = nwocg.UD2 * 0.36;
  nwocg.S3 = nwocg.S2 - nwocg.G1;

  nwocg.UD2 = nwocg.S1;

  nwocg.UD3 = nwocg.S3;

  nwocg.UD1 = nwocg.S2;
}

static const nwocg_ExtPort
  ext_ports[] =
{
  { "out", &nwocg.S3, 0 },
  { "in", &nwocg.in, 1 },
  { 0, 0, 0 }
};

const nwocg_ExtPort * const
  nwocg_generated_ext_ports = ext_ports;

const size_t
  nwocg_generated_ext_ports_size = sizeof(ext_ports);
