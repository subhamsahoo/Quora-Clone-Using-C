//done using cygwin terminal
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#define bit_vector 16384
#define block_size 512
int sockfd,newsockfd,len;
FILE *fp;
char input_user[20];
char category_gname[100];
int message_id;
struct user
{
	char username[236];
	int category_offset[5];
};
struct category
{
	char categoryname[128];
	int message_offset[96];
	//int next_message_offset;
};
struct message
{
	char message_data[128];
	char username[32];
	int reply_offset[88];
	//int next_reply_offset;
};

struct reply{
	char username[30];
	char reply_data[482];
};
void add_new_category(char input_user[])
{
	struct user u;
	struct category c;
	//printf("Enter the new category name:");
	//fflush(stdin);
	int i;
	//scanf(" %[^\n]",c.categoryname);
	read(newsockfd,&c.categoryname,sizeof(c.categoryname));
	for(i=0;i<96;i++)
	c.message_offset[i]=0;
	int j=0;
	for (i = 1; i <= 20; i++)
	{
		
		
			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&u, 256, 1, fp);
			if(strcmp(u.username,input_user)==0)
			{
				for(j=0;j<5;j++)
				{
					if(u.category_offset[4]!=0)
					{
						printf("User has already enter 5 category\n");
						return;
					}
					if(u.category_offset[j]==0)
					{
						int count,count2;
						u.category_offset[j]=u.category_offset[j-1]+1;
						fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
						fwrite(&u, 256, 1, fp);
						fseek(fp, (512 * 138) + (512 * (u.category_offset[j] - 1)) + 1, SEEK_SET);
						fwrite(&c,block_size,1,fp);
						fseek(fp,8388605,SEEK_SET);
						fread(&count,4,1,fp);
						count2=count+1;
						fseek(fp,8388605,SEEK_SET);
						fwrite(&count2,4,1,fp);
						printf("\nCategory is successfully added\n");
						break;
					}
				}
			}
		}
	
}
int findbit()
{
	int bit,bitdata,bitwrite=1;
	for(bit=1;bit<=bit_vector;bit++)
	{
		
			fseek(fp,(4*(bit-1))+1,SEEK_SET);
			fread(&bitdata,sizeof(int),1,fp);
			if(bitdata==0){
				fseek(fp,(4*(bit-1))+1,SEEK_SET);
				fwrite(&bitwrite,sizeof(int),1,fp);
				return bit;	
		}
	}
		
	}


void add_new_message(char input_user[],char categoryname_input[])
{
	struct message m;
	struct user user_display;
	struct category category_display;
	int j=0;
	//printf("Enter a message:");
	//fflush(stdin);
	//scanf(" %[^\n]",m.message_data);
	read(newsockfd,&m.message_data,sizeof(m.message_data));
	int i;
	for(i=0;input_user[i]!='\0';i++)
	{
		m.username[j++]=input_user[i];
	}
	for(i=0;i<88;i++)
	{
		m.reply_offset[i]=0;
	}
	int freeblock=findbit();
	//printf("freeblockno=%d\n",freeblock);
	fseek(fp,(238*512)+(512*(freeblock-1))+1,SEEK_SET);
	fwrite(&m,block_size,1,fp);
int k=0,l;
for (i = 1; i <= 20; i++)
	{
		
			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			for (j = 0; j < 5; j++)
			{
				if (user_display.category_offset[j] != 0)
				{
					fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
					fread(&category_display, block_size, 1, fp);
					if(strcmp(category_display.categoryname,categoryname_input)==0)
					{
							for(l=1;l<=96;l++)
							{
								if(category_display.message_offset[l]==0)
								{
									category_display.message_offset[l]=freeblock;
									fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
									fwrite(&category_display, block_size, 1, fp);
									printf("\nmessage is successfully added\n");
									break;
								}
							}
					}
				}
				else
				{
					break;
				}

			
			}
		}
}
void add_new_reply(char input_user[],char categoryname_input[])
{
	struct message m;
	struct user user_display;
	struct category category_display;
	struct reply r;
	//int message_id;
	int count=0;
	int j=0,i;
	//printf("Enter the message id for reply:");
	//scanf("%d",&message_id);
	read(newsockfd,&message_id,sizeof(message_id));
	/*printf("Enter the reply:");
	fflush(stdin);
	scanf(" %[^\n]",r.reply_data);*/
	read(newsockfd,&r.reply_data,sizeof(r.reply_data));
	for(i=0;input_user[i]!='\0';i++)
	{
		r.username[j++]=input_user[i];
	}
	int freeblock=findbit();
	printf("freeblockno=%d\n",freeblock);
	fseek(fp,(238*512)+(512*(freeblock-1))+1,SEEK_SET);
	fwrite(&r,block_size,1,fp);
	int k,l,rep;
	for (i = 1; i <= 20; i++)
	{
		
			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			for(k=0;k<5;k++)
			{
				if(user_display.category_offset[k]!=0)
				{
					fseek(fp,(138*512)+(512*(user_display.category_offset[k]-1))+1,SEEK_SET);
					fread(&category_display,block_size,1,fp);
					if(strcmp(category_display.categoryname,categoryname_input)==0)
					{
						for(l=1;l<=96;l++)
						{
							if(category_display.message_offset[l]!=0)
							{
								count++;
								if(count==message_id)
								{
									fseek(fp,(512*238)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
									fread(&m,block_size,1,fp);
									for(rep=1;rep<=88;rep++)
									{
										if(m.reply_offset[rep]==0)
										{
											m.reply_offset[rep]=freeblock;
											fseek(fp,(512*238)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
											fwrite(&m,block_size,1,fp);
											printf("\nReply is successfully added\n");
											break;
										}
									}
								}
							}
						}
					}
				}
				else{
					break;
				}
			}
	}
	
	

}

void display_replys()
{
	struct message m;
	struct user user_display;
	struct category category_display;
	struct reply r;
	//int message_id;
	int count=0,found=0;
	int id=0;
	int j=0,i,k,l,rep;
	//printf("Enter the message id for reply:");
	//scanf("%d",&message_id);
	read(newsockfd,&message_id,sizeof(message_id));
	for (i = 1; i <= 20; i++)
	{
		fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			for(k=0;k<5;k++)
			{
				if(user_display.category_offset[k]!=0)
				{
					fseek(fp,(138*512)+(512*(user_display.category_offset[k]-1))+1,SEEK_SET);
					fread(&category_display,block_size,1,fp);
					if(strcmp(category_display.categoryname,category_gname)==0)
					{
						for(l=1;l<=96;l++)
						{
							if(category_display.message_offset[l]!=0)
							{
								count++;
								if(count==message_id)
								{
									fseek(fp,(512*238)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
									fread(&m,block_size,1,fp);
									for(rep=0;rep<=88;rep++)
									{
											if(m.reply_offset[rep]!=0)
											{
												found=1;
												fseek(fp,(238*512)+(512*(m.reply_offset[rep]-1))+1,SEEK_SET);
												fread(&r,block_size,1,fp);
												write(newsockfd,&r,sizeof(r));
												/*printf("Reply id=%d\n",(++id));
												printf("%s\n",r.username);
												printf("%s\n\n",r.reply_data);*/
											}
									}
									if(found==0)
									{
										printf("No reply in this section\n");
									}
								}
								
							}
						}
					}
				}
			}
	}
	
		memset(&r,0, sizeof(r));
	write(newsockfd,&r,sizeof(r));
}


void display_category()
{
	struct user user_display;
	struct category category_display;
	int j,i;
	//printf("\n\n\t\t\tThis are the topics in the discussion\n\n\n");
	for (i = 1; i <= 20; i++)
	{

			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			for (j = 0; j < 5; j++)
			{
				if (user_display.category_offset[j] != 0)
				{
					fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
					fread(&category_display, block_size, 1, fp);
					write(newsockfd,&category_display,sizeof(category_display));
					//printf("%s\n", category_display.categoryname);
					//sleep(1);
							
				}
				else
				{
					break;
				}

		}
	}

}
void display_messages()
{
	struct user user_display;
	struct category category_display;
	struct message m;
	int j,count=0,i,l;
	int choice;
	int message_id=0;

	for (i = 1; i <= 20; i++)
	{

			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			//printf("%d.%s\n", i, user_display.username);
			for (j = 0; j < 5; j++)
			{
				if (user_display.category_offset[j] != 0)
				{
					fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
					fread(&category_display, block_size, 1, fp);
					//printf("%s\n", category_display.categoryname);
					if(strcmp(category_display.categoryname,category_gname)==0)
					{
						
					for(l=0;l<96;l++)
							{
								if(category_display.message_offset[l]!=0)
								{
									count=1;
									fseek(fp,(238*512)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
									fread(&m,block_size,1,fp);
									write(newsockfd,&m,sizeof(m));
									//printf("Message id=%d\n",(++message_id));
									//printf("%s\n",m.username);
									//printf("%s\n\n\n",m.message_data);
								}
							}
							if(count==0)
							{
								printf("\nNo message or discusion in the category\n");
							}
							
				}
				}
				else
				{
					break;
				}

		}
	}
	//m=NULL;
	memset(&m,0, sizeof(m));
	write(newsockfd,&m,sizeof(m));

}

void delete_reply()
{
	int reply_id;
	int bit=0;
	//printf("Enter the reply id for deletion:");
	//scanf("%d",&reply_id);
	read(newsockfd,&reply_id,sizeof(reply_id));
	struct message m;
	struct user user_display;
	struct category category_display;
	struct reply r;
	//int message_id;
	int count=0,count2=0,found=0;
	int id=0;
	int j=0,i,k,l,rep;
	for (i = 1; i <= 20; i++)
	{
		fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			for(k=0;k<5;k++)
			{
				if(user_display.category_offset[k]!=0)
				{
					fseek(fp,(138*512)+(512*(user_display.category_offset[k]-1))+1,SEEK_SET);
					fread(&category_display,block_size,1,fp);
					if(strcmp(category_display.categoryname,category_gname)==0)
					{
						for(l=1;l<=96;l++)
						{
							if(category_display.message_offset[l]!=0)
							{
								count++;
								if(count==message_id)
								{
									fseek(fp,(512*238)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
									fread(&m,block_size,1,fp);
									for(rep=0;rep<=88;rep++)
									{
											if(m.reply_offset[rep]!=0)
											{
												count2++;
												if(count2==reply_id)
												{
													found=1;
													m.reply_offset[rep]=0;
													fseek(fp,(512*238)+(512*(category_display.message_offset[l]-1))+1,SEEK_SET);
													fwrite(&m,block_size,1,fp);
													int b=m.reply_offset[rep];
													fseek(fp,((4*(b-1))+1),SEEK_SET);
													fwrite(&bit,4,1,fp);
													printf("\nReply is successfully deleted\n");
													break;
												}
											}
									}
									if(found==0)
									{
										printf("No reply in this section\n");
									}
								}
								
							}
						}
					}
				}
				else
				{
					break;
				}
			}
	}
}
void delete_message()
{
	int id;
	//printf("Enter the message id for deletion:");
	//scanf("%d",&id);
	read(newsockfd,&id,sizeof(id));
	struct user user_display;
	struct category category_display;
	struct message m;
	struct reply r;
	int j,i,l,count=0,found=0,bit=0,t;
	for (i = 1; i <= 20; i++)
	{

			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			//printf("%d.%s\n", i, user_display.username);
			for (j = 0; j < 5; j++)
			{
				if (user_display.category_offset[j] != 0)
				{
					fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
					fread(&category_display, block_size, 1, fp);
					//printf("%s\n", category_display.categoryname);
					if(strcmp(category_display.categoryname,category_gname)==0)
					{
						
					for(l=0;l<96;l++)
							{
								if(category_display.message_offset[l]!=0)
								{
									count++;
									if(count==id)
									{
										int store=category_display.message_offset[l];
										category_display.message_offset[l]=0;
										fseek(fp, (512 * 138) + (512 * (user_display.category_offset[j] - 1)) + 1, SEEK_SET);
										fwrite(&category_display,block_size,1,fp);
										fseek(fp,(238*512)+(512*(store-1))+1,SEEK_SET);
										fread(&m,block_size,1,fp);
										for(t=0;t<88;t++)
										{
											if(m.reply_offset[t]!=0)
											{
											fseek(fp,(4*(m.reply_offset[t]-1))+1,SEEK_SET);
											fwrite(&bit,4,1,fp);
											}
										}
										fseek(fp,(4*(store-1))+1,SEEK_SET);
											fwrite(&bit,4,1,fp);
											printf("\nmessage is successfully deleted\n");
									}
								}
							}
							
				}
				}
				else
				{
					break;
				}

		}
	}

	
}

main()
{
struct sockaddr_in ser_addr,cli_addr;
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
perror("socket error\n");
exit(1);
}
ser_addr.sin_family=AF_INET;
ser_addr.sin_port=htons(8112);
ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
if(bind(sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr))<0)
{
perror("bind error");
exit(1);
}
listen(sockfd,1);
len=sizeof(cli_addr);
printf("server is waiting for connection .....\n");
if((newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&len))<0)
{
perror("accept error");
exit(1);
}
printf("server is connected to the client......\n");
//your code hear
int count2;
fp = fopen("messaging_data.bin", "rb+");
struct user user_display;
struct category category_display;
struct message m;
int j = 0;
int choice=0,choice2,choice3,i;
	for (i = 1; i <= 20; i++)
	{	
			fseek(fp, (512 * 128) + (256 * (i - 1)) + 1, SEEK_SET);
			fread(&user_display, 256, 1, fp);
			write(newsockfd,&user_display,sizeof(user_display));
			//printf("%d.%s\n", i, user_display.username);
	}
	read(newsockfd,&choice,sizeof(choice));
	if(choice==1)
	{
	read(newsockfd,input_user,sizeof(input_user));
	}
	else
	{
		exit(0);
	}
	//printf("%s\n",input_user);
	fseek(fp,8388605,SEEK_SET);
	fread(&count2,4,1,fp);
	write(newsockfd,&count2,sizeof(count2));
	display_category();
	do{
		
		
		read(newsockfd,&choice2,sizeof(choice2));
		if(choice2==1)
		{
				//printf("Enter the category_name:");
				//fflush(stdin);
				//scanf(" %[^\n]",category_gname);
				read(newsockfd,category_gname,sizeof(category_gname));
				display_messages();	
					do{
	/*printf("1.Add a message\n");
	printf("2.SEE replys of a messages\n");
	printf("3.Add a reply\n");
	printf("4.delete a message\n");
	printf("5.delete a reply\n");
	printf("6.Back\n");
	printf("Enter your choice:");
	scanf("%d",&choice3);*/
	read(newsockfd,&choice3,sizeof(choice3));
	if(choice3==1)
	{
		add_new_message(input_user,category_gname);
		display_messages();	
	}
	else if(choice3==2)
	{
		display_messages();	
		display_replys();
		
	}
	else if(choice3==3)
	{
		display_messages();	
		add_new_reply(input_user,category_gname);
	}

	else if(choice3==4)
	{
		display_messages();
		delete_message();
		display_messages();
	}
	else if(choice3==5)
	{
		display_messages();
		display_replys();
		delete_reply();
		
	}
	}while(choice3!=6);
		}
		 if(choice2==2)
		{
			add_new_category(input_user);
			fseek(fp,8388605,SEEK_SET);
			fread(&count2,4,1,fp);
			write(newsockfd,&count2,sizeof(count2));
			display_category();
		}
		/*else if(choice==3)
		{
			exit(0);
		}*/
	}while(choice2!=3);
	fclose(fp);
close(sockfd);
}
