
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>



class JniObject {

private:
	JNIEnv* mEnv;
	std::string mClassname;
	jobject mInstance;

	jclass mCachedClassObj;

public:
	JniObject(JNIEnv* env, const char* name);
	JniObject(JNIEnv* env, jobject instance);

	JNIEnv* getJNIEnv() {
		return mEnv;
	}

	jobject getInstance() {
		return mInstance;
	}

	jclass getClass();
};

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

	const char* operator& (){
		if(!isValid) {
			mCachedCharBuf = mEnv->GetStringUTFChars(mString, NULL);
			isValid = true;
		}
		return mCachedCharBuf;
	}
};



#endif /* JNIOBJECT_H_ */