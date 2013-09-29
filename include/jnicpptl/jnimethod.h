

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <string>

class JniProxy;
class JniClass;
class JniObject;


class JniMethod_base {

protected:
	const std::string mMethodName;
	const std::string mMethodSignature;
	jmethodID mCachedMethodID;

public:
	JniMethod_base(const char* name, const char* sig)
	: mMethodName(name)
	, mMethodSignature(sig)
 	, mCachedMethodID(NULL)
	{}
};

template<class T, bool isStatic>
class JniMethod;

template<class R, typename... P, bool isStatic>
class JniMethod<R(P...), isStatic> : public JniMethod_base {

protected:
	JniClass* mClassProxy;

public:
	JniMethod(const char* name, const char* sig)
	: JniMethod_base(name, sig)
	{}

	JniMethod(JniClass* proxy, const char* name, const char* sig)
	: JniMethod_base(name, sig)
	, mClassProxy(proxy)
	{}

	JNIEnv* getJNIEnv();
	jclass getClass();
	jmethodID getMethodID();

	R operator() (P... q);
};

template<class R, typename... P>
class JniMethod<R(P...), false> : public JniMethod_base {
protected:
	JniObject* mInstanceProxy;

public:
	JniMethod(const char* name, const char* sig)
	: JniMethod_base(name, sig)
	{}

	JniMethod(JniObject* proxy, const char* name, const char* sig)
	: JniMethod_base(name, sig)
	, mInstanceProxy(proxy)
	{}

	JNIEnv* getJNIEnv();
	jclass getClass();
	jmethodID getMethodID();

	const jobject getInstance();

	R operator() (P... q) {
		return JniMethodInvoke(getJNIEnv(), getInstance(), getMethodID(), q...);
	}

};



#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<class R, typename... P> \
static TYPE JniMethodInvoke(JNIEnv* env, jobject inst, jmethodID method, P... q) { \
	return (TYPE)env->Call ## NAME_FRAG ## Method(inst, method, q...); \
} \
template<class R, typename... P> \
static TYPE JniMethodInvoke(JNIEnv* env, jclass clazz, jmethodID method, P... q) { \
	return (TYPE)env->CallStatic ## NAME_FRAG ##Method(clazz, method, q...); \
}

JniMethod_ACCESSORS(void, Void)
JniMethod_ACCESSORS(jstring, Object)
JniMethod_ACCESSORS(jobject, Object)
JniMethod_ACCESSORS(jclass, Object)
JniMethod_ACCESSORS(jboolean, Boolean)
JniMethod_ACCESSORS(jbyte, Byte)
JniMethod_ACCESSORS(jchar, Char)
JniMethod_ACCESSORS(jshort, Short)
JniMethod_ACCESSORS(jint, Int)
JniMethod_ACCESSORS(jlong, Long)
JniMethod_ACCESSORS(jfloat, Float)
JniMethod_ACCESSORS(jdouble, Double)

#undef JniMethod_ACCESSORS


#endif /* JNIMETHOD_H_ */
