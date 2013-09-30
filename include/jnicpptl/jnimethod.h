

#ifndef JNIMETHOD_H_
#define JNIMETHOD_H_

#include <jni.h>
#include <string>

#include <jnicpptl/jniproxy.h>

template<class R, typename... P>
R JniMethodInvoke(JNIEnv* env, jclass clazz, jmethodID method, P... q);

template<class R, typename... P>
R JniMethodInvoke(JNIEnv* env, jobject inst, jmethodID method, P... q);


#define JniMethod_ACCESSORS(TYPE, NAME_FRAG) \
template<class R, typename... P> \
TYPE JniMethodInvoke(JNIEnv* env, jclass clazz, jmethodID method, P... q) { \
	return (TYPE)env->CallStatic ## NAME_FRAG ##Method(clazz, method, q...); \
} \
template<class R, typename... P> \
TYPE JniMethodInvoke(JNIEnv* env, jobject inst, jmethodID method, P... q) { \
	return (TYPE)env->Call ## NAME_FRAG ## Method(inst, method, q...); \
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

	jmethodID getMethodID() {
		jmethodID retval = mMethodID;
		if(mMethodID == NULL && mProxy != NULL){
			JNIEnv* env = mProxy->getJNIEnv();
			jclass clazz = mProxy->getClass();
			if(clazz != NULL) {
				retval = env->GetStaticMethodID(clazz, mMethodName.c_str(), mMethodSignature.c_str());
				mMethodID = retval;
			}
		}
		return retval;
	}

	R operator() (P... q) {
		if(mProxy != NULL) {
			return JniMethodInvoke(mProxy->getJNIEnv(), mProxy->getClass(), getMethodID(), q...);
		}
	}
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

	jmethodID getMethodID() {
		jmethodID retval = mMethodID;
		if(mMethodID == NULL && mProxy != NULL){
			JNIEnv* env = mProxy->getJNIEnv();
			jclass clazz = mProxy->getClass();
			if(clazz != NULL) {
				retval = env->GetMethodID(clazz, mMethodName.c_str(), mMethodSignature.c_str());
				mMethodID = retval;
			}
		}
		return retval;
	}

	R operator() (P... q);

};






#endif /* JNIMETHOD_H_ */
