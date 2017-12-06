//done using cygwin terminal
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#define bit_vector 16384
#define block_size 512
int sockfd,n;
char input_user[20];
char category_gname[100];
int message_id=0;
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
void formart_display_category()
{
	int count,i;
	read(sockfd,&count,sizeof(count));
	struct category category_display;
	printf("\n\n\t\t\tThis are the topics in the discussion\n\n\n");
	for(i=1;i<=count;i++)
	{
		read(sockfd,&category_display,sizeof(category_display));
		printf("%s\n", category_display.categoryname);
	}
}
void formart_add_category()
{
		char name[128];
		printf("Enter the new category name:");
		scanf(" %[^\n]",name);
		write(sockfd,name,sizeof(name));
}
void format_display_messages()
{
	struct message m;
	int message_id=0;
	while(1)
	{
		read(sockfd,&m,sizeof(m));
		if(m.message_data[0]==(char)0)
		{
			break;
		}
		else
		{
			printf("Message id=%d\n",(++message_id));
			printf("%s\n",m.username);
			printf("%s\n\n\n",m.message_data);
		}
	}
}
void format_add_new_message()
{
	char message_data[128];
	printf("Enter the message:");
	scanf(" %[^\n]",message_data);
	write(sockfd,message_data,sizeof(message_data));
}
void format_display_replys()
{
	struct reply r;
	int mid,id=0;
	printf("Enter the message id for reply:");
	scanf("%d",&mid);
	write(sockfd,&mid,sizeof(mid));
	while(1)
	{
		read(sockfd,&r,sizeof(r));
		if(r.reply_data[0]==(char)0)
		{
			break;
		}
		else
		{
			printf("Reply id=%d\n",(++id));
			printf("%s\n",r.username);
			printf("%s\n\n",r.reply_data);
		}
		
	}
}
void format_add_new_reply()
{
	char reply_data[482];
	int mid;
	printf("Enter the message id for reply:");
	scanf("%d",&mid);
	write(sockfd,&mid,sizeof(mid));
	printf("Enter the reply:");
	fflush(stdin);
	scanf(" %[^\n]",reply_data);
	write(sockfd,reply_data,sizeof(reply_data));
}
void format_delete_message(){
	int id;
	printf("Enter the message id for deletion:");
	scanf("%d",&id);
	write(sockfd,&id,sizeof(id));
}
void format_delete_reply()
{
	int reply_id;
	printf("Enter the reply id for deletion:");
	scanf("%d",&reply_id);
	write(sockfd,&reply_id,sizeof(reply_id));
}
main()
{
struct sockaddr_in serv_addr;
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
{
perror("socket error\n");
exit(1);
}
serv_addr.sin_family=AF_INET;
serv_addr.sin_port=htons(8112);
serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//system Ip v-4 address if connected to internet
if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
{
perror("connect error");
exit(1);
}
//your code here
int i,choice,choice2,choice3;
struct user user_display;
struct category category_display;
struct message m;
for(i=1;i<=20;i++)
{
	read(sockfd,&user_display,sizeof(user_display));
	printf("%d.%s\n",i,user_display.username);
}
	printf("1.Login to the messenger\n");
	printf("2.EXIT\n");
	printf("Enter your choice:");
	scanf("%d",&choice);
	write(sockfd,&choice,sizeof(choice));
		if(choice==1)
	{
		printf("Enter the username as display in the list:");
		scanf("%s",input_user);
		write(sockfd,input_user,sizeof(input_user));		
	}
	else
	{
		exit(0);
	}
	formart_display_category();
	do{
		
		printf("1.Enter to the discussion\n");
		printf("2.Add a category\n");
		printf("3.Exit\n");
		printf("Enter your choice:");
		scanf("%d",&choice2);
		write(sockfd,&choice2,sizeof(choice2));
		if(choice2==1)
		{
				printf("Enter the category_name:");
				scanf(" %[^\n]",category_gname);
				write(sockfd,category_gname,sizeof(category_gname));
				format_display_messages();	
					do{
	printf("1.Add a message\n");
	printf("2.SEE replys of a messages\n");
	printf("3.Add a reply\n");
	printf("4.delete a message\n");
	printf("5.delete a reply\n");
	printf("6.Back\n");
	printf("Enter your choice:");
	scanf("%d",&choice3);
	write(sockfd,&choice3,sizeof(choice3));
	if(choice3==1)
	{
		format_add_new_message();
		format_display_messages();	
	}
	else if(choice3==2)
	{
		format_display_messages();	
		format_display_replys();
	}
	else if(choice3==3)
	{
		format_display_messages();	
		format_add_new_reply();
	}
	else if(choice3==4)
	{
		format_display_messages();
		format_delete_message();
		format_display_messages();
	}
	else if(choice3==5)
	{
		format_display_messages();
		format_display_replys();
		format_delete_reply();
		
	}
	}while(choice3!=6);
		}

		if(choice2==2)
		{
			formart_add_category();
			formart_display_category();
		}
			}while(choice2!=3);
close(sockfd);
}
