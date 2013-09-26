
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>

template <class T>
class JniMethod;

template<class T>
class JniObject_base {
protected:
	JNIEnv* mEnv;
	T mInstance;

private:
	std::string mClassname;

public:
	JniObject_base(const char* name)
	: mEnv(NULL)
 	, mInstance(NULL)
 	, mClassname(name)
	{}

	JniObject_base(const char* name, JNIEnv* env, T instance)
	: mEnv(env)
	, mInstance(instance)
	, mClassname(name)
	{}



	~JniObject_base() {
		if(mEnv != NULL && mInstance != NULL) {
			mEnv->DeleteLocalRef(mInstance);
		}
	}



	JNIEnv* getJNIEnv() {
		return mEnv;
	}

	const char* getClassname() {
		return mClassname.c_str();
	}

	const T& getInstance() {
		return mInstance;
	}

	operator const T& () {
		return getInstance();
	}
};


class JniObject : public JniObject_base<jobject> {

private:
	jclass mCachedClassObj;

public:
	JniObject(const char* name)
	: JniObject_base<jobject>(name)
	, mCachedClassObj(NULL)
	{};

	JniObject(const char* name, JNIEnv* env, jobject instance)
	: JniObject_base<jobject>(name, env, instance)
	, mCachedClassObj(NULL)
	{};

	jclass getClass() {
		if(mCachedClassObj == NULL){
			if(mInstance != NULL){
				mCachedClassObj = mEnv->GetObjectClass(mInstance);
			} else {
				mCachedClassObj = mEnv->FindClass(getClassname());
			}
		}
		return mCachedClassObj;
	}
};


const JniObject makeJniObject(JNIEnv* env, jobject instance);


class JniString : public JniObject_base<jstring> {
private:
	std::string mNativeString;

public:
	JniString(JNIEnv* env, jstring str)
	: JniObject_base("java/lang/String", env, str)
	{
		const char* buf = mEnv->GetStringUTFChars(str, NULL);
		mNativeString = buf;
		mEnv->ReleaseStringUTFChars(str, buf);
	}

	JniString(JNIEnv* env, const char* buf)
	: JniObject_base("java/lang/String")
	{
		mEnv = env;
		mNativeString = buf;
		mInstance = env->NewStringUTF(buf);
		
	}

	const char* getCStr() {
		return mNativeString.c_str();
	}


	operator const char*() {
		return getCStr();
	}

};



#endif /* JNIOBJECT_H_ */
