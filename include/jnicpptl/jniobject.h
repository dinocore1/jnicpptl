
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>
#include <string>

class JniProxy;

template<class T>
class JniObject_base {

protected:
	JniProxy* mProxy;

public:
	JniObject_base(JniProxy* proxy)
	: mProxy(proxy)
	{}

	~JniObject_base();

	JniProxy* getProxy() {
		return mProxy;
	}

	void setProxy(JniProxy* proxy) {
		mProxy = proxy;
	}

	const T get();
	void set(const T& value);

	operator const T () {
		return get();
	}
};

class JniClass : public JniObject_base<jclass> {

friend class JniObject;
protected:
	std::string mClassname;

public:
	JniClass(JniProxy* proxy, const char* name)
	: JniObject_base<jclass>(proxy)
	, mClassname(name)
	{}

	JniClass(const char* name)
	: JniObject_base<jclass>(NULL)
	, mClassname(name)
	{}
};

class JniObject : public JniObject_base<jobject> {

protected:
	JniClass mClass;

public:
	JniObject(JniProxy* proxy, const char* name)
	: JniObject_base<jobject>(proxy)
	, mClass(proxy, name)
	{}

	JniObject(const char* name)
	: JniObject_base<jobject>(NULL)
	, mClass(NULL, name)
	{}

	jclass getClass();
};


JniObject makeJniObject(JNIEnv* env, jobject instance);


class JniString : public JniObject_base<jstring> {
private:
	JniClass mClass;
	std::string mNativeString;

public:
	JniString(JniProxy* proxy)
	: JniObject_base(proxy)
	, mClass(proxy, "java/lang/String")
	{}

	JniString(JniProxy* proxy, const char* buf)
	: JniObject_base(proxy)
	, mClass(proxy, "java/lang/String")
	, mNativeString(buf)
	{}

	const jstring get();
	void set(const jstring& value);

	const char* getCStr();

	operator const char*() {
		return getCStr();
	}

};





#endif /* JNIOBJECT_H_ */
