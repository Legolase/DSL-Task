#include "nwocg_run.h"
#include <math.h>

static struct
{
  double in;
  double UD2;
  double UD1;
} nwocg;

void nwocg_generated_init()
{
  nwocg.UD2 = 0;
  nwocg.UD1 = 0;
}

void nwocg_generated_step()
{

  nwocg.UD2 = nwocg.UD1;

  nwocg.UD1 = nwocg.in;
}

static const nwocg_ExtPort
  ext_ports[] =
{
  { "out", &nwocg.UD2, 0 },
  { "in", &nwocg.in, 1 },
  { 0, 0, 0 }
};

const nwocg_ExtPort * const
  nwocg_generated_ext_ports = ext_ports;

const size_t
  nwocg_generated_ext_ports_size = sizeof(ext_ports);
