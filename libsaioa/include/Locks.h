/*
 * Locks.h
 *
 *  Created on: Dec 22, 2014
 *      Author: root
 */

#ifndef LOCKS_H_
#define LOCKS_H_

namespace container {

class Locks {
public:
	Locks();
	virtual ~Locks();
	int GetLock (int i);
	int GetLock01() const {
		return lock01;
	}

	void SetLock01(int LOCK01) {
		lock01 = LOCK01;
	}

	int GetLock02() const {
		return lock02;
	}

	void SetLock02(int LOCK02) {
			lock02 = LOCK02;
		}

	int GetLock03() const {
		return lock03;
	}

	void SetLock03(int LOCK03) {
		lock03 = LOCK03;
	}

	int GetLock04() const {
		return lock04;
	}

	void SetLock04(int LOCK04) {
		lock04 = LOCK04;
	}

private:
	int lock01;
	int lock02;
	int lock03;
	int lock04;
};

} /* namespace container */

#endif /* LOCKS_H_ */
