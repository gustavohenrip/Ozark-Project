#pragma once
#include "stdafx.hpp"
#include <mysql-cppconn-8/jdbc/mysql_connection.h>
#include <mysql-cppconn-8/jdbc/cppconn/driver.h>
#include <mysql-cppconn-8/jdbc/cppconn/exception.h>
#include <mysql-cppconn-8/jdbc/cppconn/resultset.h>
#include <mysql-cppconn-8/jdbc/cppconn/statement.h>
#include <mysql-cppconn-8/jdbc/cppconn/prepared_statement.h>

namespace MySQL {
	class MySQLConnection {
	public:
		bool Connect();
		void Disconnect();
		
		void Prepare(const char* Query);
		bool Execute();
		bool IsReadable();

		std::size_t GetRowCount();

		void BindParam(const char* Value);
		void BindParam(bool Value);
		void BindParam(double Value);
		void BindParam(int Value);
		void BindParam(uint32_t Value);
		void BindParam(int64_t Value);
		void BindParam(uint64_t Value);

		sql::SQLString GetString(const char* Column);
		bool GetBool(const char* Column);
		long double GetDouble(const char* Column);
		int GetInt(const char* Column);
		uint32_t GetUInt32(const char* Column);
		int64_t GetInt64(const char* Column);
		uint64_t GetUInt64(const char* Column);
	private:
		int m_ParamCount;
		sql::Driver* m_Driver;
		sql::Connection* m_Connection;
		sql::PreparedStatement* m_PreparedStatement;
		sql::ResultSet* m_ResultSet;
	};
}