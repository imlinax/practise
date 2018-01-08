#!/usr/bin/python

import json

if __name__ == "__main__":
	js = '''
		{
			"name":"wl"
		}
		'''	
	
	version_json = json.loads(js)
	version_json['check']['type'] = "abc"
