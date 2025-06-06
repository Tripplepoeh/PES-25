#ifndef ISTATUSCONTROLE_H
#define ISTATUSCONTROLE_H
#include <vector>
#include <cstdint>
#include <string>



class istatuscontrole{
	public:
	virtual void procesData(const uint8_t* data, size_t length) = 0;
	virtual void leesFifo() = 0;
    virtual void stuurBerichtFifo() = 0; 
	virtual const std::vector<uint8_t>& getResponse() const = 0;
	~istatuscontrole(){}
	
};
#endif 
