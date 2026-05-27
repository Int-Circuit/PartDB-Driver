**WARNING PROGRAM IS IN EARLY ALPHA**
# Setup
1. Run `make all`
2. Creae a `conf.yaml` file at the root of the project with the contents
```
server:
  url: # your server url (should end with */api)

  token: # your API token

  certificate: # setup https
    #Set to 0 for HTTP Connection, 1 for HTTPS
    enabled: 0
    selfSigned: 1
    location: # your cert location
# following options are not yet implemented, you can disregard them
  dict:
    isDictdwld: 0
    Dictionary:
serial:
  port:    
```
3. 
