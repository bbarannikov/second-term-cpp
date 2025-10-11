#include <iostream>
#include <fstream>
#include <random>

class DiceBase {
public:
    virtual unsigned roll() = 0;
    virtual ~DiceBase() = default;
};

class Dice : public DiceBase {
public:
    Dice(unsigned max, unsigned seed = std::random_device{}()) :
        max(max), dstr(1, max), reng(seed) {}
    
    unsigned roll() override {
        return dstr(reng);
    }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public DiceBase {
public:
    ThreeDicePool(unsigned max, unsigned seed1 = std::random_device{}(), unsigned seed2 = std::random_device{}(), unsigned seed3 = std::random_device{}()) : 
        dice1(max, seed1),
        dice2(max, seed2),
        dice3(max, seed3) {}
    
    unsigned roll() override {
        return dice1.roll() + dice2.roll() + dice3.roll();
    }

private:
    Dice dice1, dice2, dice3;
};

double expected_value(DiceBase &dice, unsigned number_of_rolls = 1000) {
    unsigned long long accum = 0;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt) {
        accum += dice.roll();
    }
    return static_cast<double>(accum) / number_of_rolls;
}

class PenaltyDice : public virtual DiceBase {
public:
    PenaltyDice(DiceBase& base_dice) : dice1(base_dice), dice2(base_dice) {}
    
    unsigned roll() override {
        return std::min(dice1.roll(), dice2.roll());
    }

private:
    DiceBase& dice1;
    DiceBase& dice2;
};

class BonusDice : public virtual DiceBase {
public:
    BonusDice(DiceBase& base_dice) : dice1(base_dice), dice2(base_dice) {}
    
    unsigned roll() override {
        return std::max(dice1.roll(), dice2.roll());
    }

private:
    DiceBase& dice1;
    DiceBase& dice2;
};

double value_probability(unsigned value, DiceBase &d, unsigned number_of_rolls = 10000) {
    unsigned count = 0;
    for (unsigned i = 0; i < number_of_rolls; ++i) {
        if (d.roll() == value) {
            ++count;
        }
    }
    return static_cast<double>(count) / number_of_rolls;
}

class DoubleDice : public PenaltyDice, public BonusDice {
public:
    DoubleDice(Dice& dice_ref) : 
        PenaltyDice(dice_ref), 
        BonusDice(dice_ref) {}
    
    unsigned roll() override {
        return PenaltyDice::roll() + BonusDice::roll();
    }
};

class DoubleDiceWithout : public DiceBase {
public:
    DoubleDiceWithout(Dice& dice_ref) : 
        penalty(dice_ref), bonus(dice_ref) {}
    
    unsigned roll() override {
        return penalty.roll() + bonus.roll();
    }

private:
    PenaltyDice penalty;
    BonusDice bonus;
};

void Data_for_Gistogramm(DiceBase &dice, const std::string name_of_file, unsigned number_of_rolls = 10000) {
    std::ofstream file("Files_with_rolls/" + name_of_file);

    for (int _ = 0; _ < number_of_rolls; ++_) {        
        file << dice.roll() << "\n";
    }
}   

int main() {
    Dice d6 = Dice(6);
    ThreeDicePool three_d6 = ThreeDicePool(6);
    Dice d100 = Dice(100);
    // PenaltyDice d100_penalty = PenaltyDice(d100);
    // BonusDice d100_bonus = BonusDice(d100);
    DoubleDice d100_double = DoubleDice(d100);
    
    std::cout << "Expected value for 1 d6 " + std::to_string(expected_value(d6)) + '\n';
    std::cout << "Expected value for 3 d6 " + std::to_string(expected_value(three_d6)) + '\n';
    std::cout << "Expected value for 2 d100 " + std::to_string(expected_value(d100_double)) + '\n';
    
    // Data_for_Gistogramm(d100, "d100_data");
    // Data_for_Gistogramm(d100_bonus, "d100_bonus_data");
    // Data_for_Gistogramm(d100_penalty, "d100_penalty_data");
    // Data_for_Gistogramm(three_d6, "3d6_data");
    Data_for_Gistogramm(d100_double, "d100_double_data");
}