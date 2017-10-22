#ifndef _SERVER_H_
#define _SERVER_H_
class FServer{
	private:
		const static int MAX_CLIENT = 1024;			//max client count
		int sClientSockets[MAX_CLIENT];
		int iClientCount;
		int sockserver;
		char buffer[1024];

		//function
		int RemoveClient(int client){
			for(int i = 0; i < this->iClientCount; i ++)
			{
				if(this->sClientSockets[i] == client)
				{
					while(i < this->iClientCount - 1)
					{
						this->sClientSockets[i] = this->sClientSockets[i + 1];
					}
					this->iClientCount --;
					return client;
				}
			}
			return -1;
		}
		int AddClient(int client){
			if(this->iClientCount < MAX_CLIENT)
			{
				this->sClientSockets[this->iClientCount++] = client;
				return 0;
			}
			return -1;
		}
		int GetMaxClient(){
			int iMax = this->sockserver;
			for(int i = 0; i < this->iClientCount; i ++)
			{
				if(this->sClientSockets[i] > iMax)
					iMax = this->sClientSockets[i];
			}
			return iMax + 1;
		}
    public:
    	void test();
    	int flisten(int port);			//listen serve
    	void wait();					//wait client

    	FServer(){
    		iClientCount = 0;
    	}
};
#endif
