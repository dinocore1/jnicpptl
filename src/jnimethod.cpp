
#include <jnicpptl/jnimethod.h>
#include <jnicpptl/jniproxy.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>


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

/****** Static JniMethod *******/

template<class R, typename... P>
jmethodID JniMethod<R(P...), true>::getMethodID()
{
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

template<class R, typename... P>
R JniMethod<R(P...), true>::operator() (P... q)
{
	if(mProxy != NULL) {
		return JniMethodInvoke(mProxy->getJNIEnv(), mProxy->getClass(), getMethodID(), q...);
	}
}


/******** Instance JniMethod ********/
template<class R, typename... P>
jmethodID JniMethod<R(P...), false>::getMethodID()
{
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

template<class R, typename... P>
R JniMethod<R(P...), false>::operator() (P... q)
{
	if(mProxy != NULL) {
		return JniMethodInvoke(mProxy->getJNIEnv(), mProxy->getInstance(), getMethodID(), q...);
	}
}

