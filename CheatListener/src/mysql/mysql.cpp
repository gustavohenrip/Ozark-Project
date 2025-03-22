#include "mysql.hpp"

namespace MySQL {
	bool MySQLConnection::Connect() {
		m_Driver = get_driver_instance();
		m_PreparedStatement = nullptr;
		m_ResultSet = nullptr;
		
		try {
			m_Connection = m_Driver->connect("209.141.34.159", "drugs", "uh2~^sZ5kx}muwQ;w>efF*G^LWE5K}");
			if (m_Connection) {
				m_Connection->setSchema("ozark");
				return true;
			}

			std::cout << "MySQL err: Couldn't connect!" << std::endl;
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}

		return false;
	}

	void MySQLConnection::Disconnect() {
		if (m_Connection) {
			try {
				m_Connection->close();
			} catch (sql::SQLException& error) {
				std::cout << "MySQL err: " << error.what() << std::endl;
			}

			delete m_Connection;
		}

		if (m_PreparedStatement) {
			delete m_PreparedStatement;
		}

		if (m_ResultSet) {
			delete m_ResultSet;
		}
	}

	void MySQLConnection::Prepare(const char* Query) {
		if (!m_Connection) {
			std::cout << "no connection on prepare()" << std::endl;
			return;
		}

		m_PreparedStatement = m_Connection->prepareStatement(Query);
		m_ParamCount = 0;
	}

	bool MySQLConnection::Execute() {
		if (!m_Connection || !m_PreparedStatement) {
			std::cout << "no connection on Execute()" << std::endl;
			return false;
		}

		try {
			m_PreparedStatement->execute();
			m_ResultSet = m_PreparedStatement->getResultSet();
			return true;
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
			return false;
		}
	}

	bool MySQLConnection::IsReadable() {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			std::cout << "failed read!" << std::endl;
			return false;
		}

		return m_ResultSet->next();
	}

	std::size_t MySQLConnection::GetRowCount() {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->rowsCount();
	}

	void MySQLConnection::BindParam(const char* Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setString(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(bool Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setBoolean(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(double Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setDouble(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(int Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setInt(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(uint32_t Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setUInt(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(int64_t Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setInt64(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	void MySQLConnection::BindParam(uint64_t Value) {
		if (!m_Connection || !m_PreparedStatement) {
			return;
		}

		m_ParamCount++;

		try {
			m_PreparedStatement->setUInt64(m_ParamCount, Value);
		} catch (sql::SQLException& error) {
			std::cout << "MySQL err: " << error.what() << std::endl;
		}
	}

	sql::SQLString MySQLConnection::GetString(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return "";
		}

		return m_ResultSet->getString(Column);
	}

	bool MySQLConnection::GetBool(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return false;
		}

		return m_ResultSet->getBoolean(Column);
	}

	long double MySQLConnection::GetDouble(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->getDouble(Column);
	}

	int MySQLConnection::GetInt(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->getInt(Column);
	}

	uint32_t MySQLConnection::GetUInt32(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->getUInt(Column);
	}

	int64_t MySQLConnection::GetInt64(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->getInt64(Column);
	}

	uint64_t MySQLConnection::GetUInt64(const char* Column) {
		if (!m_Connection || !m_PreparedStatement || !m_ResultSet) {
			return 0;
		}

		return m_ResultSet->getUInt64(Column);
	}
}