#include "stdafx.h"
#include "BBServer.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"
#include "DbHelper.h"

DatabaseJobManager* GDatabaseJobManager = nullptr ;


/// �Ʒ� �Լ��� DB ó�� �����忡�� �ҷ��� �Ѵ�
void DatabaseJobManager::ExecuteDatabaseJobs()
{
	assert( LThreadType == THREAD_DATABASE ) ;

	DatabaseJobContext* jobContext = nullptr ;
	while ( mDbJobRequestQueue.PopFront(jobContext) )
	{
		/// ���⼭ DBȣ���ؼ� ó���ϰ� 

		jobContext->mSuccess = jobContext->OnExecute() ;

		/// �� ����� result queue�� ��� ����
		mDbJobResultQueue.PushBack(jobContext) ;
	}
}

/// �Ʒ� �Լ��� Ŭ���̾�Ʈ ó�� �����忡�� �ҷ��� �Ѵ�
bool DatabaseJobManager::PushDatabaseJobRequest(DatabaseJobContext* jobContext)
{
	assert( LThreadType == THREAD_CLIENT ) ;
	return mDbJobRequestQueue.PushBack(jobContext) ;
}

/// �Ʒ� �Լ��� Ŭ���̾�Ʈ ó�� �����忡�� �ҷ��� �Ѵ�
bool DatabaseJobManager::PopDatabaseJobResult(DatabaseJobContext*& jobContext)
{
	assert( LThreadType == THREAD_CLIENT ) ;
	return mDbJobResultQueue.PopFront(jobContext) ;
}