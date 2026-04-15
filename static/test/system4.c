#include "nwocg_run.h"
#include <math.h>

static struct
{
  double S1;
  double M2;
  double UD2;
  double M1;
  double UD1;
  double in;
} nwocg;

void nwocg_generated_init()
{
  nwocg.UD2 = 0;
  nwocg.UD1 = 0;
}

void nwocg_generated_step()
{
  nwocg.M2 = nwocg.UD2 * 0.34;
  nwocg.S1 = nwocg.UD2 - nwocg.M2;

  nwocg.M1 = nwocg.UD1 * 3.6;
  nwocg.UD2 = nwocg.M1;

  nwocg.UD1 = nwocg.in;
}

static const nwocg_ExtPort
  ext_ports[] =
{
  { "res", &nwocg.S1, 0 },
  { "in", &nwocg.in, 1 },
  { 0, 0, 0 }
};

const nwocg_ExtPort * const
  nwocg_generated_ext_ports = ext_ports;

const size_t
  nwocg_generated_ext_ports_size = sizeof(ext_ports);
