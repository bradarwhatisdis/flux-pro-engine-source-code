/* { // FrictionProperties
    {MCNumber=d}
    {MCNumber=d}
    {MCNumber=d}
    {MCNumber=d}
    {MCNumber=d}
    {MCNumber=d}
    {MCNumber=d}
}

{ // FrictionProperties
    _coefficientOfSlidingFriction {MCNumber="mValue"d}
    _coefficientOfRollingFriction {MCNumber="mValue"d}
    _coefficientOfSpinningFriction {MCNumber="mValue"d}
    _timeOfequilibriumFactor {MCNumber="mValue"d}
    _velocityReductionSlidingFactor {MCNumber="mValue"d}
    _velocityReductionRollingFactor {MCNumber="mValue"d}
    _deltaSpinFactor {MCNumber="mValue"d}
} */

struct FrictionProperties {
    double _coefficientOfSlidingFriction; // 0x0 0.20000000000000001
    double _coefficientOfRollingFriction; // 0x8 0.0111
    double _coefficientOfSpinningFriction; // 0x10 0.025000000000000001
    double _timeOfequilibriumFactor; // 0x18 0.0014577259475218659
    double _velocityReductionSlidingFactor; // 0x20 196
    double _velocityReductionRollingFactor; // 0x28 10.878
    double _deltaSpinFactor; // 0x30 9.8000000000000007
};
