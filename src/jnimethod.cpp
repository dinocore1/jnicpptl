
#include <jnicpptl/jnimethod.h>
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniexception.h>


template<class R, typename... P, bool isStatic>
jmethodID JniMethod<R(P...), isStatic>::getMethodID()
{
	if(mCachedMethodID == NULL){
		mCachedMethodID = getJNIEnv()->GetMethodID(getClass(), mMethodName.c_str(), mMethodSignature.c_str());
		if(mCachedMethodID == NULL){
			throwErrorNow(getJNIEnv(), "unable to find method %s. (%s)", mMethodName.c_str(), mMethodSignature.c_str());
		}
	}
	return mCachedMethodID;
}

template<class R, typename... P, bool isStatic>
jclass JniMethod<R(P...), isStatic>::getClass()
{
	return mClassProxy->get();
}

template<class R, typename... P>
const jobject JniMethod<R(P...), false>::getInstance()
{
	return mInstanceProxy->get();
}


