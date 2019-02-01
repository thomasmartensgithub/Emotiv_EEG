[{"id":"43ad3e83.ff703","type":"tab","label":"Flow 1","disabled":false,"info":""},{"id":"dbb6ca72.5b4fa8","type":"function","z":"43ad3e83.ff703","name":"Lift - Threshold 60","func":"var spoken = context.get('spoken') || false;\nvar threshold = 60;\nvar outString = \"Lift\";\nvar outMsg = {payload: \"\", text : outString};\n\nif (!spoken) {\n    if (msg.payload >= threshold) {\n        outMsg.payload = outString;\n        spoken = true;\n    }\n}\n\nif (msg.payload < threshold) {\n    spoken = false;\n    outMsg.text = \"\";\n}\n\nconsole.log(outMsg);\n\ncontext.set('spoken', spoken);\nreturn outMsg;","outputs":1,"noerr":0,"x":772,"y":244,"wires":[["a2314259.4cf57","ffc23010.06e95"]]},{"id":"55f05c45.f3c704","type":"function","z":"43ad3e83.ff703","name":"Drop - Threshold 60","func":"var spoken = context.get('spoken') || false;\nvar threshold = 60;\nvar outString = \"Drop\";\nvar outMsg = {payload: \"\", text : outString};\n\nif (!spoken) {\n    if (msg.payload >= threshold) {\n        outMsg.payload = outString;\n        spoken = true;\n    }\n}\n\nif (msg.payload < threshold) {\n    spoken = false;\n    outMsg.text = \"\";\n}\n\ncontext.set('spoken', spoken);\nreturn outMsg;","outputs":1,"noerr":0,"x":765,"y":364,"wires":[["a2314259.4cf57","ffc23010.06e95"]]},{"id":"a2314259.4cf57","type":"ui_text","z":"43ad3e83.ff703","group":"4e4f4e30.8ff6c","order":0,"width":0,"height":0,"name":"","label":"Text Response","format":"{{msg.text}}","layout":"row-spread","x":1033,"y":244,"wires":[]},{"id":"ffc23010.06e95","type":"ui_audio","z":"43ad3e83.ff703","name":"Voice Response","group":"4e4f4e30.8ff6c","voice":"en-US","always":"","x":1034,"y":364,"wires":[]},{"id":"834dbfbd.fa78a","type":"EMOTIV","z":"43ad3e83.ff703","name":"EMOTIV","x":65,"y":301,"wires":[["2dc4f601.53b49a"]]},{"id":"d01e3b3.28e69c8","type":"Mental-Command","z":"43ad3e83.ff703","name":"Mental Commands","actionName":"lift","mcSen":"10","x":505,"y":176,"wires":[["dbb6ca72.5b4fa8"]]},{"id":"41458436.e3cc6c","type":"Mental-Command","z":"43ad3e83.ff703","name":"Mental Commands","actionName":"drop","mcSen":"10","x":506,"y":427,"wires":[["55f05c45.f3c704"]]},{"id":"2dc4f601.53b49a","type":"Profile-Name","z":"43ad3e83.ff703","name":"Profile Name","profileName":"RIIICK","x":264,"y":300,"wires":[["d01e3b3.28e69c8","41458436.e3cc6c"]]},{"id":"4e4f4e30.8ff6c","type":"ui_group","name":"Group 1","tab":"ace2761b.2f9728","order":1,"disp":true,"width":6},{"id":"ace2761b.2f9728","type":"ui_tab","name":"Tab 1","icon":"dashboard","order":1}]