#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  __pp_vec_float unit_value, unit_out;
  __pp_vec_float unit_max = _pp_vset_float(9.999999f);
  __pp_vec_float unit_one_float = _pp_vset_float(1.f);
  __pp_vec_float unit_zero_float = _pp_vset_float(0.f);

  __pp_vec_int unit_expo;
  __pp_vec_int unit_zero_int = _pp_vset_int(0);
  __pp_vec_int unit_one_int = _pp_vset_int(1);

  __pp_mask load_mask, expo_eq_zero, iter_mask, reverse_mask, val_eq_zero, bigger_than_max;

  int init_len, num_of_ones;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    init_len = (i + VECTOR_WIDTH > N) ? N % VECTOR_WIDTH : VECTOR_WIDTH;
    load_mask = _pp_init_ones(init_len);
    iter_mask = _pp_init_ones(init_len);
    expo_eq_zero = _pp_init_ones(0);
    val_eq_zero = _pp_init_ones(0);
    bigger_than_max = _pp_init_ones(0);

    _pp_vload_float(unit_value, values + i, load_mask); // unit_value[i] = value[i]
    _pp_vload_int(unit_expo, exponents + i, load_mask); // unit_value[i] = expo[i]

    // init output to one first
    _pp_vmove_float(unit_out, unit_one_float, load_mask); // output[i] = 1.0

    // check the value is zero or not
    _pp_veq_float(val_eq_zero, unit_value, unit_zero_float, load_mask); // if value[i] == 0
    _pp_vmove_float(unit_out, unit_zero_float, val_eq_zero);            //  output[i] = 0.0

    // check the expo is zero or not
    _pp_veq_int(expo_eq_zero, unit_expo, unit_zero_int, load_mask); // if expo[i] == 0
    _pp_vmove_float(unit_out, unit_one_float, expo_eq_zero);        // output[i] = 1.0

    // position which already decide its final number should be skip during process
    reverse_mask = _pp_mask_not(expo_eq_zero);
    iter_mask = _pp_mask_and(iter_mask, reverse_mask);
    reverse_mask = _pp_mask_not(val_eq_zero);
    iter_mask = _pp_mask_and(iter_mask, reverse_mask);

    // main calculation
    num_of_ones = _pp_cntbits(iter_mask);
    while (num_of_ones > 0)
    {
      _pp_vmult_float(unit_out, unit_out, unit_value, iter_mask); // output[i] = output[i] * value[i]
      // minus one the position that proceed calculation
      _pp_vsub_int(unit_expo, unit_expo, unit_one_int, iter_mask); // expo[i] --
      // check the output is bigger than 9.9999 or not
      _pp_vgt_float(bigger_than_max, unit_out, unit_max, iter_mask);
      // set the bigger one to 9.99
      _pp_vmove_float(unit_out, unit_max, bigger_than_max); // output[i] = 9.99999f
      // set the position to done
      reverse_mask = _pp_mask_not(bigger_than_max);      // reverse mask
      iter_mask = _pp_mask_and(iter_mask, reverse_mask); // update iter_mask
      // check the iteration is done or not
      _pp_veq_int(expo_eq_zero, unit_expo, unit_zero_int, load_mask); // if expo[i] == 0
      // update the iter_mask
      reverse_mask = _pp_mask_not(expo_eq_zero);         // reverse mask
      iter_mask = _pp_mask_and(iter_mask, reverse_mask); // update iter_mask

      // UPDATE !!!! number of iter mask
      num_of_ones = _pp_cntbits(iter_mask);
    }
    // store the result
    _pp_vstore_float(output + i, unit_out, load_mask);
  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //
  __pp_vec_float unit_value, unit_store;
  __pp_mask load_mask;
  int init_len;
  float ret = 0.f;
  unit_store = _pp_vset_float(0.f);
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    // check length to load
    init_len = (i + VECTOR_WIDTH > N) ? N % VECTOR_WIDTH : VECTOR_WIDTH;
    load_mask = _pp_init_ones(init_len);
    _pp_vload_float(unit_value, values + i, load_mask); // unit_value[i] = value[i]
    _pp_vadd_float(unit_store, unit_store, unit_value, load_mask); // store[i] = store[i]+value[i]
  }

  for (int j = VECTOR_WIDTH; j > 1; j /= 2)
  {
    _pp_hadd_float(unit_store, unit_store);
    _pp_interleave_float(unit_store, unit_store);
  }
  ret += unit_store.value[0];
  return ret;
}