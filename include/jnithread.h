
#ifndef JNITHREAD_H_
#define JNITHREAD_H_

#include <jni.h>

class JNIThreadEnv {
private:
	JavaVM* mJvm;
	JNIEnv* mEnv;
public:
	JNIThreadEnv(JavaVM* jvm)
	 : mJvm(jvm)
	{
		mJvm->AttachCurrentThread(&mEnv, NULL);
	}

	~JNIThreadEnv()
	{
		mJvm->DetachCurrentThread();
	}

	JNIEnv* operator&() {
		return mEnv;
	}

	JNIEnv* operator->() {
		return mEnv;
	}

};

class JNIMonitor {
private:
	JNIEnv* mEnv;
	jobject mObj;
	bool isLocked;

public:
	JNIMonitor(JNIEnv* env, jobject jobj)
	: mEnv(env),
	isLocked(false),
	mObj(jobj) {
		lock();
	}

	~JNIMonitor() {
		unlock();
	}

	void lock() {
		if(!isLocked){
			mEnv->MonitorEnter(mObj);
		}
	}

	void unlock() {
		if(isLocked){
			mEnv->MonitorExit(mObj);
		}
	}

};

#endif /* JNITHREAD_H_ */