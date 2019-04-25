//
// Created by OneCarrot on 2019-04-14.
//

#include "BigInteger.h"
#include <cmath>
#include <stdlib.h>
#include "sstream"
#ifndef MIN
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#define RADIX 16

map<string, char> binaryToHex = {
        {"0000",'0'}, {"0001",'1'}, {"0010",'2'}, {"0011",'3'}, {"0100",'4'}, {"0101",'5'}, {"0110",'6'}, {"0111",'7'},
        {"1000",'8'}, {"1001",'9'}, {"1010",'A'}, {"1011",'B'}, {"1100",'C'}, {"1101",'D'}, {"1110",'E'}, {"1111",'F'},
};

map<char, string> hexToBinary = {
        {'0',"0000"}, {'1',"0001"}, {'2',"0010"}, {'3',"0011"}, {'4',"0100"}, {'5',"0101"}, {'6',"0110"}, {'7',"0111"},
        {'8',"1000"}, {'9',"1001"}, {'A',"1010"}, {'B',"1011"}, {'C',"1100"}, {'D',"1101"}, {'E',"1110"}, {'F',"1111"},
};


inline bool isDigit(const char ch)
{
     return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') ;
}


BigInteger::BigInteger() {}

BigInteger::BigInteger(const int val) {
    int quantity = 0;
    int temp = val;
    while(temp != 0) {
        temp = temp/10;
        quantity++;
    }

    temp = val;

    for(int i = 0; i < quantity; i++) {
        int a = temp%10;
        temp = temp/10;
        _data.push_back(getKey(a));
    }
}

BigInteger::BigInteger(const std::string &valStr) {


    for(int i = valStr.length() - 1; i>=0; i--) {
        if(isDigit(valStr.at(i))) {
            _data.push_back(valStr.at(i));
        } else {
            puts("ILLEGAL VALUE!");
            exit(EXIT_FAILURE);
        }
    }

}

std::string BigInteger:: getString() {

    std::string result;
    for(int i = _data.size()-1; i >= 0; i--) {
        result.append(1, _data[i]);
        //cout<<_data[i]<<endl;
    }
    return result;
}

std::string BigInteger::toString() {
    value = getString();
    return value;
}

void BigInteger::pushZero(int i) {
    for(int j= 0; j < i; j++ )
    _data.insert(_data.begin(),'0');

}


int BigInteger::compare(const BigInteger &li) {
    int len1 = this->_data.size();
    int len2 = li._data.size();

    if(len1 != len2)
        return (len1>len2)?1:-1;

    for(int i = len1-1; i >= 0; i--) {
        if(getValue(this->_data[i]) == getValue(li._data[i])) continue;
        return (getValue(this->_data[i])>getValue(li._data[i]))?1:-1;
    }

    return 0;

}

bool BigInteger::operator==(const BigInteger &li) {
    return compare(li) == 0;
}

bool BigInteger::operator!=(const BigInteger &li) {
    return compare(li) != 0;
}

bool BigInteger::operator<(const BigInteger &li)  {
     return compare(li) < 0;
}

bool BigInteger::operator>(const BigInteger &li) {
     return compare(li) > 0;
}

bool BigInteger::operator<=(const BigInteger &li){
     return compare(li) <= 0;
}

bool BigInteger::operator>=(const BigInteger &li){
     return compare(li) >= 0;
}


int BigInteger::getValue(char it) {
    if(it >= '0' && it <= '9')
        return it - '0';
    if(it >= 'A' && it <= 'F')
        return (it - 'A') + 10;

    return '0';
}

char BigInteger::getKey(int it) {
    if(it>= 0 && it <= 9)
        return it+'0';
    if(it>=10 && it <= 15)
        return (it-10) + 'A';

    return '0';
}

void BigInteger::trimZero() {
    for(int i = _data.size()-1; i > 0; i--) {
        if(_data[i] == '0')
            _data.pop_back();
        else
            return;
    }
}

BigInteger BigInteger::pow(int n) {

    BigInteger resultVar = *this;

    if(n == 0)
        return BigInteger("1");
    else {
        for(int i = 1; i < n; i++)
            resultVar = resultVar * (*this);
    }
    return resultVar;
}

BigInteger BigInteger::pow(BigInteger n) {

    BigInteger origin = *this;
    BigInteger resultVar = *this;
    BigInteger ZERO("0");
    BigInteger ONE("1");
    BigInteger binaryOrigin = n.toBinary(1);

    if(*this == ZERO)
        return ZERO;

    if(n == ZERO)
        return BigInteger("1");
    else {
        for(int i = binaryOrigin._data.size() - 2; i >= 0 ; i--) {
            if(binaryOrigin._data[i] == '1') {
                resultVar = resultVar*resultVar;
                resultVar = resultVar * origin;
            } else {
                resultVar = resultVar*resultVar;
            }
        }

    }

    return resultVar;

}


BigInteger BigInteger::operator+( BigInteger &li) {
    int len1 = this->_data.size();
    int len2 = li._data.size();

    int minLen = MIN(len1,len2);
    int maxLen = MAX(len1,len2);
    const BigInteger &extraBi = (len1>len2) ? (*this): li;

    BigInteger resultVar;
    int carry = 0;
    int value = 0;

    for(int i = 0; i < minLen; i++) {
        int a = getValue(this->_data[i]);
        int b = getValue(li._data[i]);

        value = (a+b+carry) % RADIX;
        carry = (a+b+carry) / RADIX;

        resultVar._data.push_back(getKey(value));

    }

    for(int i = minLen; i < maxLen; i++) {
        int a = getValue(extraBi._data[i]);

        value = (a+carry) % RADIX;
        carry = (a+carry) / RADIX;

        resultVar._data.push_back(getKey(value));

    }

    if(carry > 0)
        resultVar._data.push_back(getKey(carry));

    return resultVar;


}

BigInteger BigInteger::operator-( BigInteger &li) {

    int len1 = this->_data.size();
    int len2 = li._data.size();

    int minLen = MIN(len1,len2);
    int maxLen = MAX(len1,len2);
    const BigInteger &extra = (len1>len2) ? (*this): li;

    BigInteger resultVar;
    int carry = 0;
    int value = 0;

    if(*this < li)
        return BigInteger("0");

    for(int i = 0; i < minLen; i++) {

        int a = getValue(this->_data[i]);
        int b = getValue(li._data[i]);
        if(a < b + carry) {
            value = a + RADIX - carry - b;
            carry = 1;
        } else {
            value = a - carry - b;
            carry = 0;
        }

        resultVar._data.push_back(getKey(value));

    }

    for(int i = minLen; i < maxLen; i++) {

        int a = getValue(this->_data[i]);
        int b = getValue(li._data[i]);
        if(a < carry){
            value = a + RADIX - carry;
            carry = 1;
        } else {
            value = a - carry;
            carry = 0;
        }

        resultVar._data.push_back(getKey(value));
    }

    resultVar.trimZero();

    return resultVar;

}

BigInteger BigInteger::operator*(BigInteger &li) {
    int len1 = this->_data.size();
    int len2 = li._data.size();

    if(len1 < len2) return li.operator*(*this);

    int value; //乘积
    int carry = 0; //进位
    BigInteger resultVar("0"); //结果

    for(int i = 0; i < len2; i++) {
        BigInteger mulTemp;
        carry = 0;

        for(int tmp = 0; tmp < i; tmp++)
            mulTemp._data.push_back('0');

        for(int j = 0; j < len1; j++) {

            value = getValue(li._data[i]) * getValue(this->_data[j]) + carry;


            mulTemp._data.push_back(getKey(value % RADIX));

            carry = value/RADIX;
        }

        if(carry)
            mulTemp._data.push_back(getKey(carry));

        resultVar = resultVar.operator+(mulTemp);

    }

    return resultVar;

}

BigInteger BigInteger::operator/(BigInteger &li) {

    int len1 = this->_data.size();
    int len2 = li._data.size();

    if(*this < li) return BigInteger("0");

    int n = len1-len2;

    int result = 0;

    static BigInteger ONE("1");
    static BigInteger TEN("10");
    BigInteger resultVar("0");
    BigInteger temp = *this;

    for(int i = n; i>=0; i--) {

        BigInteger count("0");
        BigInteger a = TEN.pow(i);
        BigInteger anoLi = li;
        anoLi.pushZero(i);
        BigInteger midValue = anoLi;
        //cout<<"hello"<<endl;
        while (temp >= midValue) {
            temp = temp - midValue;
            count = count + ONE;
        }

        BigInteger b = count * a;
        resultVar = resultVar + b;

    }

    resultVar.trimZero();

    return resultVar;


}

BigInteger BigInteger::operator%(BigInteger &li) {

    BigInteger resultVar;
    BigInteger divisionResult;
    BigInteger midValue;
    BigInteger a;

    if(*this == li)
        return BigInteger("0");

    a = *this;

    divisionResult = a / li;
    midValue = divisionResult * li;
    midValue.trimZero();
    resultVar = a - midValue;

    return resultVar;

}



BigInteger BigInteger::generateRangeRand(BigInteger max) {

    BigInteger TWO("2");
    BigInteger INTMAX(INT32_MAX);
    BigInteger randNum;

    if(max < BigInteger(INT32_MAX)) {
        randNum = BigInteger(rand()) % max;
        while (randNum < TWO)
            randNum = BigInteger(rand()) % max;

    } else {
        randNum = BigInteger(rand()) % INTMAX;
        while (randNum < TWO)
            randNum = BigInteger(rand()) % INTMAX;
    }

    return randNum;

}

BigInteger BigInteger::toBinary(int flag) {

    stringstream resultss;

    for(int i = _data.size() - 1; i >= 0; i--) {
        resultss << hexToBinary[_data[i]];
    }

    BigInteger result(resultss.str());


    if(flag == 1) {
        result.trimZero();
        return result;
    } else
        return result;

}

BigInteger BigInteger::toHex() {

    stringstream resultss;

    for(int i = _data.size()-4; i >= 0; i = i - 4) {
        int value = 0;
        stringstream ss;
        for(int j = 3; j >= 0; j--) {
            ss << _data[i + j];
            string temp = ss.str();
        }
        resultss << binaryToHex[ss.str()];
    }

    string temp2 = resultss.str();
    return BigInteger(resultss.str());
}

int BigInteger::getFirst() {
    return getValue(_data[0]);
}

string BigInteger::getName() {
    return name;
}

void BigInteger::setName(string name) {
    this->name = name;
}

char BigInteger::valueOf(int index) {
    return _data[index];
}

int BigInteger::getSize() {
    return _data.size();
}

void BigInteger::cyclicShift(int num, bool direction) {  //左0右1
    if(!direction) {
        for(int i = 0; i < num; i++) {
            char tmp = _data.back();
            _data.insert(_data.begin(),tmp);
            _data.pop_back();
        }
    } else {
        for(int i = 0; i < num; i++) {
            char tmp = _data.front();
            _data.erase(_data.begin());
            _data.push_back(tmp);
        }
    }
}


BigInteger BigInteger::XOR(BigInteger b) {
    stringstream ss;
    for(int i = b.getSize() - 1; i >= 0; i--) {
        if( this->valueOf(i) == b.valueOf(i)) {
            ss << '0';
        } else {
            ss << '1';
        }
    }

    return BigInteger(ss.str());
}

