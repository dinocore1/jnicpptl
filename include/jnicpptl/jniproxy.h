#ifndef JNIPROXY_H_
#define JNIPROXY_H_

#include <jni.h>

class JniProxy {
public:
	virtual ~JniProxy(){};
	virtual JNIEnv* getJNIEnv() = 0;
	virtual jclass getClass() = 0;
	virtual jobject getInstance() = 0;

	virtual void setClass(jclass clazz) = 0;
	virtual void setInstance(jobject inst) = 0;
};

class JniDefaultProxy : public JniProxy {
public:
	JNIEnv* mEnv;
	jclass mClass;
	jobject mInstance;

	JniDefaultProxy(JNIEnv* env, jclass clazz, jobject inst)
	: mEnv(env)
	, mClass(clazz)
	, mInstance(inst)
	{}

	JniDefaultProxy();

	virtual JNIEnv* getJNIEnv() {
		return mEnv;
	}
	virtual jclass getClass() {
		return mClass;
	}
	virtual jobject getInstance(){
		return mInstance;
	}

	virtual void setClass(jclass clazz){
		mClass = clazz;
	}
	virtual void setInstance(jobject inst){
		mInstance = inst;
	}
};

class JniLocalProxy : public JniDefaultProxy {
public:
	JniLocalProxy(JNIEnv* env, jclass clazz, jobject inst)
	: JniDefaultProxy(env, clazz, inst)
	{}

	~JniLocalProxy() {
		if(mEnv != NULL && mInstance != NULL){
			mEnv->DeleteLocalRef(mInstance);
		}
	}
};

class JniStaticProxy : public JniProxy {

public:
	JniProxy* mParentProxy;
	jclass mClass;

	JniStaticProxy()
	: mClass(NULL)
	{};

	~JniStaticProxy() {
		if(mClass != NULL){
			mParentProxy->getJNIEnv()->DeleteGlobalRef(mClass);
		}
		mClass = NULL;
	}

	void setParent(JniProxy* proxy) {
		mParentProxy = proxy;
	}

	virtual JNIEnv* getJNIEnv() {
		return NULL;
	}
	virtual jclass getClass() {
		return mClass;
	}
	virtual jobject getInstance(){
		return NULL;
	}

	virtual void setClass(jclass clazz){
		if(mClass != NULL){
			mParentProxy->getJNIEnv()->DeleteGlobalRef(mClass);
		}
		mClass = (jclass)mParentProxy->getJNIEnv()->NewGlobalRef(clazz);
	}
	virtual void setInstance(jobject inst){
	}
};

#endif /* JNIPROXY_H_ */
