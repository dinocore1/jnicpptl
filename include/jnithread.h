
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

#endif /* JNITHREAD_H_ */