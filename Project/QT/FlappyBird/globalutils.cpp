#include "globalutils.h"

Globalutils::Globalutils(){

}

//获取随机数 0 ~ 200之间
int Globalutils::getRandom(uint seed,int limit){
    qsrand(seed);
    return qrand()%limit;
}
