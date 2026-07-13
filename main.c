#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char reservation_id[100];
    char room_no[100];
    char reservation_status[100];
    char customer_name[100];
    char national_id[100];
    char no_nights[100];
    char checkIn_date[100];
    char email[100];
    char phone_number[100];

} Reservation;

typedef struct
{
    char room[100];
    char room_status[100];
    char room_category[100];
    char price[100];

} Room;

void tokenize_reservation(char *line_reservation,Reservation *reservations)
{
    char *token=strtok(line_reservation,",");
    strcpy((*reservations).reservation_id,token);

    token=strtok(NULL,",");
    strcpy((*reservations).room_no,token);

    token=strtok(NULL,",");
    strcpy((*reservations).reservation_status,token);

    token=strtok(NULL,",");
    strcpy((*reservations).customer_name,token);

    token=strtok(NULL,",");
    strcpy((*reservations).national_id,token);

    token=strtok(NULL,",");
    strcpy((*reservations).no_nights,token);

    token=strtok(NULL,",");
    strcpy((*reservations).checkIn_date,token);

    token=strtok(NULL,",");
    strcpy((*reservations).email,token);

    token=strtok(NULL,",");
    strcpy((*reservations).phone_number,token);
}

void tokenize_room (char *line_room,Room *rooms)
{
    char *token=strtok(line_room," ");
    strcpy((*rooms).room,token);

    token=strtok(NULL," ");
    strcpy((*rooms).room_status,token);

    token=strtok(NULL," ");
    strcpy((*rooms).room_category,token);

    token=strtok(NULL," ");
    strcpy((*rooms).price,token);
}

void Check_In()
{
    char res_ID[100];
    char room_NO[100];
    char cust_name[100];
    char cust_ID[100];
    char nights[100];
    char current_date[100];
    char cust_email[100];
    char cust_number[100];

    getchar();
    printf("Enter the Reservation ID: \n");
    fgets(res_ID,sizeof(res_ID),stdin);
    res_ID[strcspn(res_ID,"\n")]='\0';

    printf("Enter the Room Number: \n");
    fgets(room_NO,sizeof(room_NO),stdin);
    room_NO[strcspn(room_NO,"\n")]='\0';

    printf("Enter the Customer Name: \n");
    fgets(cust_name,sizeof(cust_name),stdin);
    cust_name[strcspn(cust_name,"\n")]='\0';

    printf("Enter the Customer's National ID: \n");
    fgets(cust_ID,sizeof(cust_ID),stdin);
    cust_ID[strcspn(cust_ID,"\n")]='\0';

    printf("Enter the Number Of Nights Reserved: \n");
    fgets(nights,sizeof(nights),stdin);
    nights[strcspn(nights,"\n")]='\0';

    printf("Enter the Check-In Date \"YYYY-MM-DD\": \n");
    fgets(current_date,sizeof(current_date),stdin);
    current_date[strcspn(current_date,"\n")]='\0';

    printf("Enter the Customer's Email: \n");
    fgets(cust_email,sizeof(cust_email),stdin);
    cust_email[strcspn(cust_email,"\n")]='\0';

    printf("Enter the Customer's Phone Number: \n");
    fgets(cust_number,sizeof(cust_number),stdin);
    cust_number[strcspn(cust_number,"\n")]='\0';

    FILE *reservation=fopen("Reservation.txt","r+");
    FILE *room=fopen("Room.txt","r+");

    if (reservation==NULL || room==NULL)
    {
        printf("Error in opening the file!\n");
        exit(1);
    }

    char line_reservation[2000];
    char line_room[2000];

    Reservation reservations[100];
    Room rooms[100];

    int res_count=0;
    int room_count=0;

    int found_res=0;
    int valid_checkIn=0;

    while (fgets(line_reservation,sizeof(line_reservation),reservation)!=NULL)
    {
        tokenize_reservation(line_reservation,&reservations[res_count]);

        if (strcmp(reservations[res_count].reservation_id,res_ID)==0 &&
            strcmp(reservations[res_count].room_no,room_NO)==0 &&
            strcmp(reservations[res_count].reservation_status,"unconfirmed")==0 &&
            strcmp(reservations[res_count].customer_name,cust_name)==0 &&
            strcmp(reservations[res_count].national_id,cust_ID)==0 &&
            strcmp(reservations[res_count].no_nights,nights)==0 &&
            strcmp(reservations[res_count].checkIn_date,current_date)==0 &&
            strcmp(reservations[res_count].email,cust_email)==0 &&
            strcmp(reservations[res_count].phone_number,cust_number)==0)
        {
            found_res=1;

            if (strcmp(reservations[res_count].checkIn_date,current_date)<=0)
            {
                valid_checkIn=1;
                strcpy(reservations[res_count].reservation_status,"confirmed");
            }
        }

        res_count++;
    }

    while (fgets(line_room,sizeof(line_room),room)!=NULL)
    {
        tokenize_room(line_room,&rooms[room_count]);

        if (valid_checkIn==1 &&
            strcmp(rooms[room_count].room,room_NO)==0 &&
            strcmp(rooms[room_count].room_status,"Available")==0)
        {
            strcpy(rooms[room_count].room_status,"Reserved");
        }

        room_count++;
    }

    freopen("Reservation.txt","w",reservation);
    for (int i=0;i<res_count;i++)
    {
        fprintf(reservation,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",reservations[i].reservation_id,
                                                         reservations[i].room_no,
                                                         reservations[i].reservation_status,
                                                         reservations[i].customer_name,
                                                         reservations[i].national_id,
                                                         reservations[i].no_nights,
                                                         reservations[i].checkIn_date,
                                                         reservations[i].email,
                                                         reservations[i].phone_number);
    }

    freopen("Room.txt","w",room);
    for (int i=0;i<room_count;i++)
    {
        fprintf(room,"%s %s %s %s\n",rooms[i].room,
                                    rooms[i].room_status,
                                    rooms[i].room_category,
                                    rooms[i].price);
    }

    fclose(reservation);
    fclose(room);

    if (found_res && valid_checkIn)
        printf("Check-In Completed Successfully!\n");
    else if (found_res && !valid_checkIn)
        printf("Check-In is not allowed before the check-in date specified in the reservation data\n");
    else
        printf("Reservation data match not found!\n");
}

void Cancel_Reservation()
{
    char resID_or_roomNo[100];

    getchar();
    printf("Enter Reservation ID or Room Number: \n");
    fgets(resID_or_roomNo,sizeof(resID_or_roomNo),stdin);
    resID_or_roomNo[strcspn(resID_or_roomNo,"\n")]='\0';

    FILE *reservation=fopen("Reservation.txt","r+");
    FILE *room=fopen("Room.txt","r+");

    if (reservation==NULL || room==NULL)
    {
        printf("Error in opening the file!\n");
        exit(1);
    }

    char line_reservation[2000];
    char line_room[2000];

    Reservation reservations[100];
    Room rooms[100];

    int res_count=0;
    int room_count=0;
    int found_res=0;
    int can_cancel=0;
    char cancelled_roomNo[100];

    while (fgets(line_reservation,sizeof(line_reservation),reservation)!=NULL)
    {
        tokenize_reservation(line_reservation,&reservations[res_count]);

        if ((strcmp(reservations[res_count].reservation_id,resID_or_roomNo)==0 || strcmp(reservations[res_count].room_no,resID_or_roomNo)==0) && strcmp(reservations[res_count].reservation_status,"unconfirmed")==0)
        {
            found_res=1;
            can_cancel=1;
            strcpy(cancelled_roomNo,reservations[res_count].room_no);
            continue;
        }

        res_count++;
    }

    while(fgets(line_room,sizeof(line_room),room)!=NULL)
    {
        tokenize_room(line_room,&rooms[room_count]);

        if ((strcmp(rooms[room_count].room,resID_or_roomNo)==0 || strcmp(cancelled_roomNo,rooms[room_count].room)==0) && can_cancel)
        {
            strcpy(rooms[room_count].room_status,"Available");
        }

        room_count++;
    }

    freopen("Reservation.txt","w",reservation);
    for (int i=0;i<res_count;i++)
    {
        fprintf(reservation,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",reservations[i].reservation_id
                                                          ,reservations[i].room_no
                                                          ,reservations[i].reservation_status
                                                          ,reservations[i].customer_name
                                                          ,reservations[i].national_id
                                                          ,reservations[i].no_nights
                                                          ,reservations[i].checkIn_date
                                                          ,reservations[i].email
                                                          ,reservations[i].phone_number);


    }

    freopen("Room.txt","w",room);
    for (int i=0;i<room_count;i++)
    {
        fprintf(room,"%s %s %s %s\n",rooms[i].room
                                    ,rooms[i].room_status
                                    ,rooms[i].room_category
                                    ,rooms[i].price);
    }

    fclose(reservation);
    fclose(room);

    if (found_res && can_cancel)
        printf("Reservation Cancelled Successfully!\n");
    else if (found_res && !can_cancel)
        printf("Cannot Cancel Reservation as it was already Confirmed\n");
    else
        printf("Reservation was Not Found!\n");


}

int main()
{
    int choice;

    printf("1.Check-In\n2.Cancel Reservation\nEnter your choice: ");

    scanf("%d",&choice);

    switch (choice)
    {
        case 1:
        Check_In();
        break;

        case 2:
        Cancel_Reservation();
        break;

        default:
        printf("Invalid Choice!\n");
        break;
    }

    return 0;
}
