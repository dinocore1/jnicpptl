
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jnimethod.h>

JniObject::JniObject(const char* name)
 : mClassname(name)
 , mEnv(NULL)
 , mInstance(NULL)
 , mCachedClassObj(NULL)
{
}

JniObject::JniObject(const char* name, JNIEnv* env, jobject instance)
 : mClassname(name)
 , mEnv(env)
 , mInstance(instance)
 , mCachedClassObj(NULL)
{
}

/*
template<class T>
JniObject::JniObject(const char* name, JNIEnv* env, const JniMethod<T> &constructor)
 : mClassname(name)
 , mEnv(env)
 , mInstance(NULL)
 , mCachedClassObj(NULL)
{

}
*/

const JniObject& makeJniObject(JNIEnv* env, jobject instance)
{
	JniMethod<jstring()> getName("getName", "()Ljava/lang/String;");
	JniString className(env, getName());

	JniObject retval(className, env, instance);
}

jclass JniObject::getClass()
{
	if(mCachedClassObj == NULL){
		if(mInstance != NULL){
			mCachedClassObj = mEnv->GetObjectClass(mInstance);
		} else {
			mCachedClassObj = mEnv->FindClass(mClassname.c_str());
		}
	}
	return mCachedClassObj;
}