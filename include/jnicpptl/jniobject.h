
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>

template <class T>
class JniMethod;

class JniObject {

protected:
	JNIEnv* mEnv;
	jobject mInstance;

private:
	std::string mClassname;
	jclass mCachedClassObj;

public:
	JniObject(const char* name);
	JniObject(const char* name, JNIEnv* env, jobject instance);

	//template<class T>
	//JniObject(const char* name, JNIEnv* env, const JniMethod<T> &constructor);

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


class JniString : public JniObject {
private:
	std::string mNativeString;

public:
	JniString(JNIEnv* env, jstring str)
	: JniObject("java/lang/String", env, (jobject)str)
	{
		const char* buf = mEnv->GetStringUTFChars(str, NULL);
		mNativeString = buf;
		mEnv->ReleaseStringUTFChars(str, buf);
	}

	JniString(JNIEnv* env, const char* buf)
	: JniObject("java/lang/String")
	{
		mEnv = env;
		mNativeString = buf;
		mInstance = (jobject)env->NewStringUTF(buf);
		
	}

	const char* getCStr() {
		return mNativeString.c_str();
	}

	jstring getJString() {
		return (jstring)mInstance;
	}

	operator const char*() {
		return getCStr();
	}

	operator jstring() {
		return getJString();
	}

};



#endif /* JNIOBJECT_H_ */