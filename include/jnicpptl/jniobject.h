
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>



class JniObject {

private:
	std::string mClassname;
	JNIEnv* mEnv;
	jobject mInstance;

	jclass mCachedClassObj;

public:
	JniObject(const char* name);
	JniObject(const char* name, JNIEnv* env, jobject instance);

	JNIEnv* getJNIEnv() {
		return mEnv;
	}

	jobject getInstance() {
		return mInstance;
	}

	operator jobject () {
		return getInstance();
	}

	jclass getClass();
};

const JniObject& makeJniObject(JNIEnv* env, jobject instance);


class JniString {
private:
	JNIEnv* mEnv;
	jstring mString;
	bool isValid;
	const char* mCachedCharBuf;

public:
	JniString(JNIEnv* env, jstring str)
	: mEnv(env)
	, mString(str)
	, isValid(false)
	{}

	~JniString()
	{
		if(isValid){
			mEnv->ReleaseStringUTFChars(mString, mCachedCharBuf);
			isValid = false;
		}
	}

	const char* get() {
		if(!isValid) {
			mCachedCharBuf = mEnv->GetStringUTFChars(mString, NULL);
			isValid = true;
		}
		return mCachedCharBuf;
	}

	operator const char*() {
		return get();
	}

};



#endif /* JNIOBJECT_H_ */