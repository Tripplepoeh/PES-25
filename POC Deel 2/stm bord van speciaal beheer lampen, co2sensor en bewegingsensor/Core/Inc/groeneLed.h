/*
 * groeneLed.h
 *
 *  Created on: May 19, 2025
 *      Author: aashi
 */

#ifndef GROENELED_H_
#define GROENELED_H_
#include "lamp.h"

class groeneLed : public lamp{
public:
	groeneLed();
	void zetAan();
	void zetUit();
};



#endif /* GROENELED_H_ */
