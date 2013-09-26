
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
			isLocked = true;
		}
	}

	void unlock() {
		if(isLocked){
			mEnv->MonitorExit(mObj);
			isLocked = false;
		}
	}

};

class JniLocalFrame {
private:
	JNIEnv* mEnv;
	jobject* returnObj;

public:
	JniLocalFrame(JNIEnv* env, int numRefs)
	: mEnv(env)
	, returnObj(NULL) {
		mEnv->PushLocalFrame(numRefs);
	}

	~JniLocalFrame() {
		mEnv->PopLocalFrame(NULL);
	}



};

class JniGlobalRef {
private:
	JNIEnv* mEnv;
	jobject instance;

public:
	JniGlobalRef()
	: mEnv(NULL)
	, instance(NULL) {}

	JniGlobalRef(JNIEnv* env, jobject ref)
	: mEnv(env) {
		instance = env->NewGlobalRef(ref);
	}

	~JniGlobalRef() {
		if(mEnv != NULL && instance != NULL) {
			mEnv->DeleteGlobalRef(instance);
		}
	}

	JniGlobalRef& operator=(JniGlobalRef other) {
		std::swap(mEnv, other.mEnv);
		std::swap(instance, other.instance);
		return *this;
	}

	operator const jobject& () {
		return instance;
	}

};

#endif /* JNITHREAD_H_ */
