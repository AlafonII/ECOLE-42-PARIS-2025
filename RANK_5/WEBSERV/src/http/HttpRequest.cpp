#include "all.hpp"

/*==============================================================*/
/*                        STATIC HELPERS                        */
/*==============================================================*/

/*
 * Function: hexval
 * Role: Convert a hexadecimal character to its integer value.
 *
 * Parameters:
 *   c - Hexadecimal digit (0-9, a-f, A-F).
 *
 * Returns:
 *   int - Numeric value or -1 if the character is invalid.
 */
static int	hexval(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + (c - 'a');
	if (c >= 'A' && c <= 'F')
		return 10 + (c - 'A');
	return -1;
}

/*
 * Function: percent_decode
 * Role: Decode percent-encoded characters within a URI segment.
 *
 * Parameters:
 *   in - Input string to decode.
 *   out - Output string receiving decoded characters.
 *
 * Returns:
 *   bool - true on success, false if an invalid encoding is found.
 */
static bool	percent_decode(const std::string &in, std::string &out)
{
	size_t i = 0;
	out.clear();
	while (i < in.size())
	{
		if (in[i] == '%')
		{
			if (i + 2 >= in.size())
				return false;
			int h1 = hexval(in[i + 1]);
			int h2 = hexval(in[i + 2]);
			if (h1 < 0 || h2 < 0)
				return false;
			// Convert the two-digit hex sequence into a single character.
			char ch = static_cast<char>((h1 << 4) | h2);
			out.push_back(ch);
			i += 3;
		}
		else
		{
			out.push_back(in[i]);
			i++;
		}
	}
	return true;
}

/*==============================================================*/
/*                        CONSTRUCTORS                          */
/*==============================================================*/
/*
 * Function: HttpRequest::HttpRequest
 * Role: Initialize an empty request structure with basic MIME mappings.
 */
HttpRequest::HttpRequest(void)
{
	// Preload a small set of common extensions for Content-Type inference.
	this->_mimeTypes[".html"] = "text/html";
	this->_mimeTypes[".htm"] = "text/html";
	this->_mimeTypes[".txt"] = "text/plain";
	this->_mimeTypes[".jpg"] = "image/jpeg";
	this->_mimeTypes[".jpeg"] = "image/jpeg";
	this->_mimeTypes[".png"] = "image/png";
	this->_mimeTypes[".pdf"] = "application/pdf";
	this->_mimeTypes[".php"] = "text/html";
}

/*
 * Function: HttpRequest::~HttpRequest
 * Role: Default destructor for the request object.
 */
HttpRequest::~HttpRequest(void)
{
}

/*==============================================================*/
/*                            GETTERS                           */
/*==============================================================*/

/*
 * Function: HttpRequest::getMethod
 * Role: Return the parsed HTTP method string.
 *
 * Returns:
 *   std::string& - Reference to the stored method.
 */
std::string&	HttpRequest::getMethod( void )
{
	return (this->_method);
}


/*
 * Function: HttpRequest::getPath
 * Role: Access the decoded request path.
 *
 * Returns:
 *   std::string& - Reference to the stored path.
 */
std::string&	HttpRequest::getPath( void )
{
	return (this->_path);
}


/*
 * Function: HttpRequest::getHeaders
 * Role: Provide direct access to the parsed header map.
 *
 * Returns:
 *   std::map<std::string, std::string>& - Reference to the headers container.
 */
std::map<std::string, std::string>	&HttpRequest::getHeaders( void )
{
	return (this->_headers);
}

/*
 * Function: HttpRequest::getMimeType
 * Role: Infer the MIME type based on the request path extension.
 *
 * Returns:
 *   std::string - MIME type string, defaulting to text/html.
 */
std::string	HttpRequest::getMimeType(void)
{
	std::string	ext;
	size_t		dot = this->_path.find_last_of('.');

	if (dot != std::string::npos)
	{
		ext = this->_path.substr(dot);
		if (this->_mimeTypes.count(ext))
			// Return the known type when the extension is registered.
			return (this->_mimeTypes[ext]);
	}
	return "text/html";
}


/*
 * Function: HttpRequest::getBody
 * Role: Access the parsed request body.
 *
 * Returns:
 *   std::string& - Reference to the stored body.
 */
std::string&	HttpRequest::getBody( void )
{
	return (this->_body);
}


/*
 * Function: HttpRequest::getRequest
 * Role: Retrieve the raw request buffer as accumulated from the socket.
 *
 * Returns:
 *   std::string& - Reference to the raw request string.
 */
std::string&	HttpRequest::getRequest( void )
{
	return (this->_request);
}


/*
 * Function: HttpRequest::getQuery
 * Role: Access the parsed query string.
 *
 * Returns:
 *   std::string& - Reference to the query component.
 */
std::string&	HttpRequest::getQuery( void )
{
	return (this->_query);
}


/*
 * Function: HttpRequest::getHeader
 * Role: Look up a specific header by name.
 *
 * Parameters:
 *   key - Header field name to search for.
 *
 * Returns:
 *   std::string - Header value or empty string if not present.
 */
std::string	HttpRequest::getHeader(const std::string& key)
{
	std::map<std::string, std::string>::iterator it = this->_headers.find(key);
	if (it == this->_headers.end())
		return std::string();
	return it->second;
}


/*
 * Function: HttpRequest::getIsGoodSyntax
 * Role: Report whether the last parse attempt succeeded.
 *
 * Returns:
 *   bool - true if the request syntax was valid.
 */
bool	HttpRequest::getIsGoodSyntax( void )
{
	return (this->_isGoodSyntax);
}

/*==============================================================*/
/*                            SETTERS                           */
/*==============================================================*/

/*
 * Function: HttpRequest::setIsGoodSyntax
 * Role: Update the parsing status flag.
 *
 * Parameters:
 *   goodSyntax - true if parsing succeeded.
 */
void	HttpRequest::setIsGoodSyntax( bool goodSyntax )
{
	this->_isGoodSyntax = goodSyntax;
}


/*==============================================================*/
/*                     BUFFER MANAGEMENT                        */
/*==============================================================*/

/*
 * Function: HttpRequest::appendToRequest
 * Role: Append raw bytes read from the socket to the request buffer.
 *
 * Parameters:
 *   data - Pointer to the incoming bytes.
 *   len - Number of bytes to append.
 */
void HttpRequest::appendToRequest(const char* data, size_t len)
{
	// Grow the raw buffer with the newly read bytes.
	this->_request.append(data, len);
}


/*==============================================================*/
/*             HTTP PARSING / RESPONSE BUILDING                 */
/*==============================================================*/

/*
 * Function: HttpRequest::parse
 * Role: Decode the raw HTTP request into structured fields and body, handling chunked payloads.
 *
 * Parameters:
 *   raw - Raw HTTP request data including headers and body.
 *
 * Returns:
 *   bool - true if parsing succeeds with valid syntax.
 */
bool HttpRequest::parse(const std::string& raw)
{
	this->_method.clear();
	this->_path.clear();
	this->_version.clear();
	this->_headers.clear();
	this->_body.clear();
	this->_query.clear();
	this->_isGoodSyntax = false;

	// Detect the blank line that separates headers from the body payload
	std::string::size_type head_end = raw.find("\r\n\r\n");
	if (head_end == std::string::npos)
		return false;

	std::string head = raw.substr(0, head_end);
	std::string body = raw.substr(head_end + 4);
	this->_body = body;

	// split request-line
	std::string::size_type line_end = head.find("\r\n");
	std::string request_line;
	if (line_end == std::string::npos)
		request_line = head;
	else
		request_line = head.substr(0, line_end);

	std::istringstream rl(request_line);
	if (!(rl >> this->_method >> this->_path >> this->_version))
		return false;

	// HTTP/1.1 only
	if (this->_version != "HTTP/1.1")
		return false;

	// origin-form target must start with '/' for our server
	if (this->_path.empty() || this->_path[0] != '/')
		return false;

	// Separate the path from the query string
	std::string raw_target = this->_path;
	std::string::size_type q = raw_target.find('?');
	std::string raw_path;
	if (q == std::string::npos)
	{
		raw_path = raw_target;
		this->_query.clear();
	}
	else
	{
		raw_path = raw_target.substr(0, q);
		this->_query = raw_target.substr(q + 1);
	}

	// percent-decode the path
	std::string decoded;
	if (!percent_decode(raw_path, decoded))
		return false;

	this->_path = decoded;

	// parse headers
	std::string headers_block;
	if (line_end == std::string::npos)
		headers_block = "";
	else
		headers_block = head.substr(line_end + 2);

	std::istringstream hs(headers_block);
	std::string line;
	bool chunked = false;
	while (std::getline(hs, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			break;
		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;
		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
			value.erase(0, 1);
		// Store the canonical header key and its trimmed value
		this->_headers[key] = value;
		std::string lowerKey = key;
		for (size_t i = 0; i < lowerKey.size(); ++i)
		{
			if (lowerKey[i] >= 'A' && lowerKey[i] <= 'Z')
				lowerKey[i] = static_cast<char>(lowerKey[i] - 'A' + 'a');
		}
		if (lowerKey == "transfer-encoding")
		{
			std::string lowerValue = value;
			for (size_t i = 0; i < lowerValue.size(); ++i)
			{
				if (lowerValue[i] >= 'A' && lowerValue[i] <= 'Z')
					lowerValue[i] = static_cast<char>(lowerValue[i] - 'A' + 'a');
			}
			if (lowerValue.find("chunked") != std::string::npos)
				chunked = true;
		}
	}

	if (chunked)
	{
		std::string	decoded;
		size_t		pos = 0;

		while (true)
		{
			size_t	sizeEnd = body.find("\r\n", pos);
			if (sizeEnd == std::string::npos)
				return (false);
			std::string	sizeStr = body.substr(pos, sizeEnd - pos);
			std::istringstream sizeStream(sizeStr);
			size_t	chunkSize = 0;
			sizeStream >> std::hex >> chunkSize;
			if (!sizeStream)
				return (false);
			pos = sizeEnd + 2;
			if (chunkSize == 0)
				break;
			if (pos + chunkSize > body.size())
				return (false);
			decoded.append(body, pos, chunkSize);
			pos += chunkSize;
			if (pos + 1 >= body.size() || body.substr(pos, 2) != "\r\n")
				return (false);
			pos += 2;
		}
		this->_body = decoded;
	}

	this->_isGoodSyntax = true;
	return true;
}


/*
 * Function: HttpRequest::clear
 * Role: Reset the request to an empty state, ready for new data.
 */
void HttpRequest::clear(void)
{
	// Reset every field so the parser can process the next request cleanly.
	this->_request.clear();
	this->_method.clear();
	this->_path.clear();
	this->_version.clear();
	this->_headers.clear();
	this->_body.clear();
	this->_query.clear();
	this->_isGoodSyntax = false;
}
