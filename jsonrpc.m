function object = jsonrpc( url, method, params, auth )

    
    request = struct();
    object = struct();
    request.jsonrpc = '2.0';
    request.method = method;
    request.params = params;
    request.id = 'matlab';

    if (nargin > 3)
        request.auth = auth;
    end
    
    jsonrequest = tojson(request);
    jsonresponse = jsonrpcpost(jsonrequest, url);
    response = fromjson(jsonresponse);
    
    if isfield(response, 'error') 
        throw(MException('JSONRPC:error', response.error.message))
    else
        object = response.result;
    end
end

