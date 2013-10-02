

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <string>


#include <jnicpptl/jniproxy.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>


template<bool isStatic>
class JniMethod_base {

protected:
	const std::string mMethodName;
	const std::string mMethodSignature;
	jmethodID mMethodID;
	JniProxy* mProxy;

public:
	JniMethod_base(JniProxy* proxy, const char* name, const char* sig)
	: mMethodName(name)
	, mMethodSignature(sig)
 	, mMethodID(NULL)
	, mProxy(proxy)
	{}

	jmethodID getMethodID();
};

template<>
jmethodID JniMethod_base<true>::getMethodID() {
	if(mMethodID == NULL) {
		mMethodID = mProxy->getJNIEnv()->GetStaticMethodID(mProxy->getClass(), mMethodName.c_str(), mMethodSignature.c_str());
	}
	return mMethodID;
}

template<>
jmethodID JniMethod_base<false>::getMethodID() {
	if(mMethodID == NULL) {
		mMethodID = mProxy->getJNIEnv()->GetMethodID(mProxy->getClass(), mMethodName.c_str(), mMethodSignature.c_str());
	}
	return mMethodID;
}

template<class T, bool isStatic>
class JniMethod;


#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<typename... P> \
class JniMethod<TYPE(P...), false> : public JniMethod_base<false> { \
public:	\
	JniMethod(JniProxy* proxy, const char* name, const char* sig) \
	: JniMethod_base<false>(proxy, name, sig) \
	{} \
	TYPE operator() (P... q) { \
		return (TYPE)mProxy->getJNIEnv()->Call ## NAME_FRAG ## Method(mProxy->getInstance(), getMethodID(), q...); \
	} \
}; \
template<typename... P> \
class JniMethod<TYPE(P...), true> : public JniMethod_base<true> { \
public:	\
	JniMethod(JniProxy* proxy, const char* name, const char* sig) \
	: JniMethod_base<true>(proxy, name, sig) \
	{} \
	TYPE operator() (P... q) { \
		return (TYPE)mProxy->getJNIEnv()->CallStatic ## NAME_FRAG ## Method(mProxy->getClass(), getMethodID(), q...); \
	} \
};


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
