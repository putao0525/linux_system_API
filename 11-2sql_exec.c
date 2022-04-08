char *errmsg;
if(sqlite3_exec(db, "create table stu(id int, name char, score 		int)", 
			NULL, NULL, &errmsg) < 0){
	printf("error: %s\n", errmsg);
}
			
