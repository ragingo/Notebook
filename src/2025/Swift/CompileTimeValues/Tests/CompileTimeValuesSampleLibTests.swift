import CompileTimeValuesSampleLib

@const let add_result = Math.add(2, 3)
@const let add_assertion = static_assert(add_result == 5)

@const let sub_result = Math.sub(5, 3)
@const let sub_assertion = static_assert(sub_result == 2)

@const let mul_result = Math.mul(2, 3)
@const let mul_assertion = static_assert(mul_result == 6)

@const let div_result = Math.div(6, 3)
@const let div_assertion = static_assert(div_result == 2)

@const let abs_result = Math.abs(-5)
@const let abs_assertion = static_assert(abs_result == 5)

@const let min_result = Math.min(2, 3)
@const let min_assertion = static_assert(min_result == 2)

@const let max_result = Math.max(2, 3)
@const let max_assertion = static_assert(max_result == 3)

@const let clamp_result = Math.clamp(value: 5, from: 2, to: 4)
@const let clamp_assertion = static_assert(clamp_result == 4)

@const let toRad_result = Math.toRad(deg: 180)
@const let toRad_assertion = static_assert(toRad_result == Math.pi)

@const let toDeg_result = Math.toDeg(rad: Math.pi)
@const let toDeg_assertion = static_assert(toDeg_result == 180.0)
