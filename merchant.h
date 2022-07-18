#ifndef __MERCHANT_H_
#define __MERCHANT_H_
#include "npc.h"
#include "subject.h"

class Merchant : public NPC, public Subject {
public: 
    static int hatred; // 0 if not attacked; 1 if attacked
    static void resetHatred();
    static void setHatred();
    Merchant(Coordinate cdn);
    std::string getName() const override;
    int attack(int NPCDefence) const override;
};

#endif
