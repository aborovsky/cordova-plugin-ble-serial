# BLE Central + Serial Plugin for Apache Cordova

**This fork is for a private project and I can't provide support, sorry! YMMV depending on your BLE module and phone.**

This is a fork of Don Coleman's [BLE Central Plugin](https://github.com/don/cordova-plugin-ble-central)
with added support for simulating UART serial with a BLE module that supports it, such as HM-10,
RedBear, Bluefruit and others. The modifications are based on Don's [Bluetooth Serial Plugin](https://github.com/don/BluetoothSerial)
but this fork supports BLE for both Android and iOS.

If you're sending and receiving messages under 20 bytes, you can follow the examples in the original project.
Above 20 bytes, update notifications may be received out of order, especially while scrolling in iOS, and
writes may be cropped to 20 bytes.
 
This fork adds a `startSerial` method, which is similar to `startNotification` but aggregates update
notifications in native code until a delimiter is received. It also adds a `writeSerial` method, which splits data
into 20 byte packets and writes each packet in turn with a sleep between each. This is very slow, so
sending short messages is preferable.

If you only need iOS support, or can use Bluetooth Classic on Android, please use [cordova-plugin-bluetooth-serial](https://github.com/don/BluetoothSerial).

## Limitations

* Limit messages to under 200 bytes if possible. Above this, corruption and disconnects can occur depending on your hardware.
* Incoming messages must have a delimiter. Messages are not forwarded to the web view until it's received.
* The maximum incoming message length is 512 bytes. Any message longer than this is dropped. Fork to customise.
* Transmitting data to a BLE module is very slow and may be unreliable. There is a 200ms sleep between each 20 byte "packet". On some hardware this may not be sufficient.
* Implemented for iOS and Android only.

## Tips

* Begin and end each message with a terminator. This helps prevent a message getting added to the end of a previous one that was not transmitted properly.
* Add length and checksum to each message for verification.
* Include a nonce in each message and return it with the reply to match request/response pairs.
* If order of delivery to your app is critical, include a timestamp or sequence number.

**Please see the original [BLE Central Plugin](https://github.com/don/cordova-plugin-ble-central) for complete documentation.**

## Added Methods

- [ble.writeSerial](#writeSerial)
- [ble.startSerial](#startSerial)
- [ble.stopSerial](#stopSerial)

## writeSerial

Writes 20 byte packets of data to a characteristic.

    ble.writeSerial(device_id, service_uuid, characteristic_uuid, data, success, failure);

### Description

Function `writeSerial` writes 20 byte packets from the given data to a characteristic, with
a 200ms sleep between each. Messages should be as short as possible, and no longer than 200 bytes.

### Parameters
- __device_id__: UUID or MAC address of the peripheral
- __service_uuid__: UUID of the BLE service
- __characteristic_uuid__: UUID of the BLE characteristic
- __data__: binary data, use an [ArrayBuffer](#typed-arrays)
- __success__: Success callback function that is invoked when the connection is successful. [optional]
- __failure__: Error callback function, invoked when error occurs. [optional]

### Quick Example

Use an [ArrayBuffer](#typed-arrays) when writing data.

    // send 1 byte to switch a light on
    var data = new Uint8Array(1);
    data[0] = 1;
    ble.writeSerial(device_id, "FF10", "FF11", data.buffer, success, failure);

    // send a 3 byte value with RGB color
    var data = new Uint8Array(3);
    data[0] = 0xFF; // red
    data[1] = 0x00; // green
    data[2] = 0xFF; // blue
    ble.writeSerial(device_id, "ccc0", "ccc1", data.buffer, success, failure);

    // send a 32 bit integer
    var data = new Uint32Array(1);
    data[0] = counterInput.value;
    ble.writeSerial(device_id, SERVICE, CHARACTERISTIC, data.buffer, success, failure);

## startSerial

Starts a simulated serial connection.

    ble.startSerial(device_id, service_uuid, characteristic_uuid, delimiter, success, failure);

### Description

Function `startSerial` starts listening to characteristic value updates, concatenating successive
values until the given delimiter is received, then forwarding the message to the success callback.
The success callback is called multiple times.

Raw data is passed from native code to the success callback as an ArrayBuffer.

### Parameters

- __device_id__: UUID or MAC address of the peripheral
- __service_uuid__: UUID of the BLE service
- __characteristic_uuid__: UUID of the BLE characteristic
- __delimiter__: Message delimiter. Must be a single byte.
- __success__: Success callback function invoked every time a notification occurs
- __failure__: Error callback function, invoked when error occurs. [optional]

### Quick Example

    var onData = function(buffer) {
        // Decode the ArrayBuffer into a typed Array based on the data you expect
        var data = new Uint8Array(buffer);
        alert("Button state changed to " + data[0]);
    }

    ble.startSerial(device_id, "FFE0", "FFE1", "\n", onData, failure);

## stopSerial

Stop receiving serial data from aggregated characteristic changes.

    ble.stopSerial(device_id, service_uuid, characteristic_uuid, success, failure);

### Description

Function `stopSerial` stops a previously registered serial callback.

### Parameters

- __device_id__: UUID or MAC address of the peripheral
- __service_uuid__: UUID of the BLE service
- __characteristic_uuid__: UUID of the BLE characteristic
- __success__: Success callback function that is invoked when the notification is removed. [optional]
- __failure__: Error callback function, invoked when error occurs. [optional]

# License

Apache 2.0
