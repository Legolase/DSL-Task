#include "nwocg_run.h"
#include <math.h>

static struct
{
  double A3;
  double D2;
  double D1;
  double A2;
  double A1;
  double b;
  double a;
} nwocg;

void nwocg_generated_init()
{
  nwocg.D2 = 0;
  nwocg.D1 = 0;
}

void nwocg_generated_step()
{
  nwocg.A1 = -nwocg.D1 + nwocg.a;
  nwocg.A2 = nwocg.b + nwocg.D2;
  nwocg.A3 = nwocg.A1 + nwocg.A2;

  nwocg.D2 = nwocg.A2;

  nwocg.D1 = nwocg.A1;
}

static const nwocg_ExtPort
  ext_ports[] =
{
  { "result", &nwocg.A3, 0 },
  { "a", &nwocg.a, 1 },
  { "b", &nwocg.b, 1 },
  { 0, 0, 0 }
};

const nwocg_ExtPort * const
  nwocg_generated_ext_ports = ext_ports;

const size_t
  nwocg_generated_ext_ports_size = sizeof(ext_ports);
