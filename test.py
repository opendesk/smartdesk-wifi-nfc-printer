import requests

r = requests.get('http://www.smartdesk.cc/actions')
json_data = r.json()
if json_data:
    for k,v in json_data.items():
        url = 'http://arduinoyunone.local/mailbox/{0}'.format(''.join(v.split(',')[0]).split(':')[-1].replace('"','').replace(' ','%20'))
        requests.post(url)
