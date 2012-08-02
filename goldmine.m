classdef goldmine < handle
    % goldmine json-rpc library for matlab
    
    properties
        url;
        auth = '';
        version;
    end
    
    methods
        
        function self = goldmine(url)
            self.url = url;
            self.version = self.call('version');
        end
        
        function authenticate(self, username, password)
            self.auth = '';
            self.auth = self.call('authenticate', {username, password});
        end
                
        function object = call(self, method, params)
            % method: string for method
            % params (optional): n-by-1 cell array
            
            request = struct();
            object = struct();
            request.jsonrpc = '2.0';
            request.method = method;
                        
            if nargin < 3
                request.params = {};
            else
                request.params = params;
            end
            
            request.id = 'matlab';

            if (~isempty(self.auth))
                request.auth = self.auth;
            end

            jsonrequest = tojson(request);
            jsonresponse = jsonrpcpost(jsonrequest, self.url);          
            response = fromjson(jsonresponse);

            if isfield(response, 'error') 
                throw(MException('JSONRPC:Error', response.error.message))
            else
                object = response.result;
            end
            
        end
        
        
    end
    
end

