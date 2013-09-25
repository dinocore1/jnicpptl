
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

const JniObject& makeJniObject(JNIEnv* env, jobject instance)
{
	JniMethod<jstring()> getName("getName", "()Ljava/lang/String;");
	JniString className(env, getName());

	JniObject retval(className, env, instance);
}

jclass JniObject::getClass()
{
	if(mCachedClassObj != NULL){
		mCachedClassObj = mEnv->FindClass(mClassname.c_str());
	}
	return mCachedClassObj;
}