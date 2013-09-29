
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jniproxy.h>
#include <jnicpptl/jnimethod.h>

template<class T>
JniObject_base<T>::~JniObject_base()
{
	if(mProxy != NULL) {
		delete mProxy;
	}
}

template<>
const jclass JniObject_base<jclass>::get()
{
	jclass retval = NULL;
	if(mProxy != NULL) {
		retval = mProxy->getClass();
	}
	return retval;
}

template<>
void JniObject_base<jclass>::set(const jclass& value)
{
	if(mProxy != NULL){
		mProxy->setClass(value);
	}
}

template<>
const jobject JniObject_base<jobject>::get()
{
	jobject retval = NULL;
	if(mProxy != NULL) {
		retval = mProxy->getInstance();
	}
	return retval;
}

template<>
void JniObject_base<jobject>::set(const jobject& value)
{
	if(mProxy != NULL){
		mProxy->setInstance(value);
	}
}

jclass JniObject::getClass()
{
	jclass retval = NULL;
	if(mProxy != NULL){
		retval = mProxy->getClass();
		if(retval == NULL){
			jobject ins = mProxy->getInstance();
			JNIEnv* env = mProxy->getJNIEnv();
			if(env != NULL){
				if(ins != NULL){
					retval = env->GetObjectClass(ins);
				} else {
					retval = env->FindClass(mClass.mClassname.c_str());
				}
				mProxy->setClass(retval);
			}
		}
	}
	return retval;
}

const char* JniString::getCStr()
{
	if(mNativeString.empty() && mProxy != NULL){
		JNIEnv* env = mProxy->getJNIEnv();
		jstring inst = get();
		if(env != NULL && inst != NULL){
			const char* buf = env->GetStringUTFChars(inst, NULL);
			mNativeString = std::string(buf);
			env->ReleaseStringUTFChars(inst, buf);
		}
	}
	return mNativeString.c_str();
}

const jstring JniString::get()
{
	jstring retval = NULL;
	if(mProxy != NULL){
		retval = (jstring)mProxy->getInstance();
		if(retval == NULL){
			JNIEnv* env = mProxy->getJNIEnv();
			retval = env->NewStringUTF(mNativeString.c_str());
			mProxy->setInstance(retval);
		}
	}
	return retval;
}

void JniString::set(const jstring& value)
{
	mNativeString.clear();
	if(mProxy != NULL){
		mProxy->setInstance(value);
	}
}


JniObject makeJniObject(JNIEnv* env, jobject instance)
{
	JniDefaultProxy* proxy = new JniDefaultProxy(env, NULL, instance);
	JniMethod<jstring(), false> getName(proxy, "getName", "()Ljava/lang/String;");

	JniString className(new JniLocalProxy(env, NULL, getName()));

	JniObject retval(proxy, className);
	return retval;
}


