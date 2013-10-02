




/****** Static JniMethod *******/

/*
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
*/

/******** Instance JniMethod ********/

/*
template<class R, typename... P>
jmethodID JniMethod<false, R, P...>::getMethodID()
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


*/
