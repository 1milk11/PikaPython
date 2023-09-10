import mqtt
import time

client = mqtt.MQTT('broker.emqx.io', port=1883,
                   clinetID='clientid', username='name_', password='passwd_')

ret = client.connect()
print("connect ret:%d" % ret)


def callback0(evt):
    print("py0 cb: %s-qos:%d-->>%s" % (evt.topic, evt.qos, evt.msg))


def callback1(evt):
    print("py1 cb: %s-qos:%d-->>%s" % (evt.topic, evt.qos, evt.msg))


def callback2(evt):
    print("py2 cb: %s-qos:%d-->>%s" % (evt.topic, evt.qos, evt.msg))


def reconnect_mq(signal):
    print('lost mqtt connect and try to reconnect')
    print('signal:', signal)


client.setKeepAlive(5)
ret = client.subscribe('topic_pikapy_qos0', callback0, 0)
print("subscribe ret:%d" % ret)
ret = client.subscribe('topic_pikapy_qos1', callback1, 1)
print("subscribe ret:%d" % ret)
ret = client.subscribe('topic_pikapy_qos2', callback2, 2)
print("subscribe ret:%d" % ret)

client._fakeMsg("topic_pikapy_qos0", 0, "hello qos0")
client._fakeMsg("topic_pikapy_qos1", 1, "hello qos1")
client._fakeMsg("topic_pikapy_qos2", 2, "hello qos2")

# sleep wait for recv data
# T = PikaStdDevice.Time()
T = time
# T.sleep_s(5)

out = client.listSubscribeTopic()
print('listSubscribeTopic out', out)

# client.unsubscribe('topic_pikapy_qos0');
# client.unsubscribe('topic_pikapy_qos1');
# client.unsubscribe('topic_pikapy_qos2');
# T.sleep_s(5)
# out2 = client.listSubscribeTopic()
# print('listSubscribeTopic out2',out2)

ret = client.setDisconnectHandler(reconnect_mq)
print("setDisconnectHandler:%d" % ret)

# ret = client.setWill('topic_will','lost mqtt connect')
# print("setWill:%d" % ret)
# client.publish('topic_will', 'hello pikascript', 1)
# T.sleep_s(5)
# print("sleep_s:5s")

# T.sleep_s(30)
# exit()
ret = client.disconnect()
print("disconnect ret:%d" % ret)
