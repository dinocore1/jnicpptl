

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <string>

class JniProxy;

template<class R, typename... P>
R JniMethodInvoke(JNIEnv* env, jclass clazz, jmethodID method, P... q);

template<class R, typename... P>
R JniMethodInvoke(JNIEnv* env, jobject inst, jmethodID method, P... q);


class JniMethod_base {

protected:
	const std::string mMethodName;
	const std::string mMethodSignature;
	jmethodID mMethodID;

public:
	JniMethod_base(const char* name, const char* sig)
	: mMethodName(name)
	, mMethodSignature(sig)
 	, mMethodID(NULL)
	{}
};

template<class T, bool isStatic>
class JniMethod;

template<class R, typename... P>
class JniMethod<R(P...), true> : public JniMethod_base {

protected:
	JniProxy* mProxy;

public:
	JniMethod(JniProxy* proxy, const char* name, const char* sig)
	: JniMethod_base(name, sig)
	, mProxy(proxy)
	{}

	jmethodID getMethodID();

	R operator() (P... q);
};

template<class R, typename... P>
class JniMethod<R(P...), false> : public JniMethod_base {
protected:
	JniProxy* mProxy;

public:
	JniMethod(JniProxy* proxy, const char* name, const char* sig)
	: JniMethod_base(name, sig)
	, mProxy(proxy)
	{}

	jmethodID getMethodID();

	R operator() (P... q);

};






#endif /* JNIMETHOD_H_ */
