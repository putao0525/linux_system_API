sqlite3 *db;
if(sqlite3_open("stu.db", &db) != SQLITE_OK){
	printf("error: %s\n", sqlite3_errmsg(db));
}

