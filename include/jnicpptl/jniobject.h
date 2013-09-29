
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>


template<class T>
class JniObject_base {
protected:
	std::string mClassname;
	T mInstance;

public:
	JniObject_base(const char* name, T instance)
	: mClassname(name)
	, mInstance(instance)
	{}

	JniObject_base(const char* name)
	: mClassname(name)
	, mInstance(NULL)
	{}

	const T get() {
		return mInstance;
	}

	void set(const T& i) {
		mInstance = i;
	}

	operator const T () {
		return get();
	}

	JniObject_base<T>& operator= (const T& rhs) {
		set(rhs);
		return *this;
	}
};

class JniClass : public JniObject_base<jclass> {

public:
	JniClass(const char* name, jclass instance)
	: JniObject_base<jclass>(name, instance)
	{}

	JniClass(const char* name)
	: JniObject_base<jclass>(name, NULL)
	{}
};

class JniObject : public JniObject_base<jobject> {

protected:
	JniClass mClass;

public:
	JniObject(const char* name, jobject instance)
	: JniObject_base<jobject>(name, instance)
	, mClass(name)
	{}

	JniObject(const char* name)
	: JniObject_base<jobject>(name, NULL)
	, mClass(name)
	{}

	jclass getClass(JNIEnv* env) {
		jclass retval = mClass.get();
		if(retval == NULL){
			if(mInstance != NULL) {
				retval = env->GetObjectClass(mInstance);
				mClass.set(retval);
			} else {
				retval = env->FindClass(mClassname.c_str());
				mClass.set(retval);
			}
		}
		return retval;
	}
};


const JniObject makeJniObject(JNIEnv* env, jobject instance);


class JniString : public JniObject_base<jstring> {
private:
	std::string mNativeString;

public:
	JniString(JNIEnv* env, jstring str)
	: JniObject_base("java/lang/String", str)
	{
		const char* buf = env->GetStringUTFChars(str, NULL);
		mNativeString = buf;
		env->ReleaseStringUTFChars(str, buf);
	}

	JniString(JNIEnv* env, const char* buf)
	: JniObject_base("java/lang/String")
	, mNativeString(buf)
	{
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
