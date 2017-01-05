package com.usbxyz.canbootloader;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.SpinnerAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.usbxyz.usb2xxx.USB2CAN;
import com.usbxyz.usb2xxx.USB2XXX;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    // Storage Permissions
    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static String[] PERMISSIONS_STORAGE = {Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.WRITE_EXTERNAL_STORAGE };
    private static final int REQUEST_GET_FIRMWARE_FILE = 1;
    private static final int REQUEST_GET_CAN_NODE = 2;
    USB2XXX usb2xxx;
    Spinner baudRate,newBaudRate;
    Button selectCANNode,selectFirmwareFile,upgradeCANNodeFirmware,setNewBaudRate;
    EditText startAddr,endAddr,firmwareFile,nodeAddr;
    TextView upgrateStatus;
    CheckBox selectAllNode;
    private List<String> baudRateList = new ArrayList<String>();
    ProgressBar mProgressBar;
    public class ConnectStateChanged implements USB2XXX.DeviceConnectStateChanged{
        @Override
        public void stateChanged(boolean connected) {
            if(connected){
                Toast.makeText(MainActivity.this, "Device Attached", Toast.LENGTH_SHORT).show();
            }else{
                Toast.makeText(MainActivity.this, "Device Detached", Toast.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        baudRateList.add("1000 Kbps");
        baudRateList.add("500 Kbps");
        baudRateList.add("200 Kbps");
        baudRateList.add("100 Kbps");
        baudRateList.add("50 Kbps");
        baudRateList.add("20 Kbps");
        baudRate = (Spinner) findViewById(R.id.spinnerBaudRate);
        baudRate.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, baudRateList));
        newBaudRate = (Spinner) findViewById(R.id.spinnerNewBaudRate);
        newBaudRate.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, baudRateList));

        upgrateStatus = (TextView) findViewById(R.id.textViewUpgrateState);
        selectAllNode = (CheckBox) findViewById(R.id.checkBoxAllNode);
        mProgressBar = (ProgressBar)findViewById(R.id.progressBar);

        nodeAddr = (EditText) findViewById(R.id.editTextNodeAddr);
        firmwareFile = (EditText) findViewById(R.id.editTextFirmwareFile);
        startAddr = (EditText) findViewById(R.id.editTextStartAddr);
        endAddr = (EditText) findViewById(R.id.editTextEndAddr);

        usb2xxx = new USB2XXX(this, new ConnectStateChanged());//需要监视设备插拔事件

        selectCANNode = (Button) findViewById(R.id.buttonSelectCANNode);
        selectCANNode.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, ScanDeviceActivity.class);
                //用Bundle携带数据
                Bundle bundle = new Bundle();
                //传递参数
                bundle.putString("startAddr", startAddr.getText().toString());
                bundle.putString("endAddr", endAddr.getText().toString());
                bundle.putString("baudRate", baudRate.getSelectedItem().toString().substring(0, baudRate.getSelectedItem().toString().length() - 5));
                //System.out.println(baudRate.getSelectedItem().toString().substring(0,baudRate.getSelectedItem().toString().length()-5));
                intent.putExtras(bundle);
                //启动Activity
                startActivityForResult(intent, REQUEST_GET_CAN_NODE);
            }
        });

        selectFirmwareFile = (Button) findViewById(R.id.buttonSelectFirmwareFile);
        selectFirmwareFile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showFileChooser();
            }
        });

        upgradeCANNodeFirmware = (Button) findViewById(R.id.buttonUpgrate);
        upgradeCANNodeFirmware.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                verifyStoragePermissions(MainActivity.this);
                //如果文件不存在，就提示错误
                File file = new File(firmwareFile.getText().toString());
                if (file.exists()) {
                    short NodeAddr = 0;
                    int FirmwareFileSize = (int) file.length();
                    byte[] firmwareData = new byte[FirmwareFileSize];
                    mProgressBar.setMax(FirmwareFileSize);
                    mProgressBar.setProgress(0);
                    //将固件文件读入缓冲器
                    try{
                        InputStream in = new FileInputStream(file);
                        in.read(firmwareData, 0, FirmwareFileSize);
                        in.close();
                        String str="";
                        for(int i=0;i<FirmwareFileSize;i++){
                            str += String.format("%02X ",firmwareData[i]);
                            if(((i+1)%16)==0){
                                str += String.format("--%04X\n",i+1);
                            }
                        }
                        //System.out.println(str);
                    }catch (Exception ep){
                        ep.printStackTrace();
                        return;
                    }
                    //初始化配置设备
                    if (!configUSB2CANDevice(0, (byte) 0, Integer.parseInt(baudRate.getSelectedItem().toString().substring(0, baudRate.getSelectedItem().toString().length() - 5))*1000)) {
                        return;
                    }
                    //判断当前所选择的节点
                    if (selectAllNode.isChecked()) {
                        NodeAddr = 0;
                    } else {
                        if (nodeAddr.getText().toString().isEmpty()) {
                            //没有选择任何节点
                            Toast.makeText(MainActivity.this, "请先选择节点！", Toast.LENGTH_SHORT).show();
                            return;
                        } else {
                            NodeAddr = (short) Integer.parseInt(nodeAddr.getText().toString());
                        }
                    }

                    //发送跳转到Bootloader的命令
                    upgrateStatus.setText("跳转到Bootloader...");
                    int ret = usb2xxx.usb2can.CAN_BL_Excute(0, (byte) 0, NodeAddr, USB2CAN.CAN_BL_BOOT);
                    if (ret != USB2CAN.CAN_SUCCESS) {
                        Toast.makeText(MainActivity.this, "执行固件跳转命令失败！", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    //延时
                    try {
                        Thread.sleep(500);
                    } catch (Exception ep) {
                        ep.printStackTrace();
                    }
                    //检测当前节点是否为Bootloader
                    if (!selectAllNode.isChecked()) {
                        int[] appVersion = new int[1], appType = new int[1];
                        ret = usb2xxx.usb2can.CAN_BL_NodeCheck(0, (byte) 0, NodeAddr, appVersion, appType, 10);
                        if (ret != USB2CAN.CAN_SUCCESS) {
                            Toast.makeText(MainActivity.this, "节点检测失败！", Toast.LENGTH_SHORT).show();
                            upgrateStatus.setText("节点检测失败！");
                            return;
                        } else {
                            if (appType[0] != USB2CAN.CAN_BL_BOOT) {
                                Toast.makeText(MainActivity.this, "当前节点固件类型不是Bootloader，请重新点击升级固件！", Toast.LENGTH_SHORT).show();
                                return;
                            }
                        }
                    }
                    //发送擦出固件命令
                    upgrateStatus.setText("擦出固件...");
                    ret = usb2xxx.usb2can.CAN_BL_Erase(0, (byte) 0, NodeAddr, FirmwareFileSize, 10000);
                    if (ret != USB2CAN.CAN_SUCCESS) {
                        Toast.makeText(MainActivity.this, "擦出固件失败！", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    if (selectAllNode.isChecked()) {
                        try {
                            Thread.sleep(1000);
                        } catch (Exception ep) {
                            ep.printStackTrace();
                        }
                    }
                    //循环发送数据
                    int AddrOffset = 0;
                    int PackSize = 512;
                    byte[] DataBuffer = new byte[PackSize];
                    for (AddrOffset = 0; AddrOffset < FirmwareFileSize; AddrOffset += PackSize) {
                        int read_data_num = (AddrOffset+PackSize)>FirmwareFileSize?FirmwareFileSize%PackSize:PackSize;
                        System.out.println("read_data_num = "+read_data_num);
                        System.out.println("AddrOffset = "+AddrOffset);
                        if(read_data_num > 0){
                            System.arraycopy(firmwareData,AddrOffset,DataBuffer,0,read_data_num);
                            String str="";
                            for(int i=0;i<read_data_num;i++){
                                str += String.format("%02X ",DataBuffer[i]);
                                if(((i+1)%16)==0){
                                    str += "\n";
                                }
                            }
                            //System.out.println(str);
                            ret = usb2xxx.usb2can.CAN_BL_Write(0, (byte) 0, NodeAddr, AddrOffset, DataBuffer, read_data_num, 1000);
                            if (ret != USB2CAN.CAN_SUCCESS) {
                                Toast.makeText(MainActivity.this, "写数据失败！", Toast.LENGTH_SHORT).show();
                                return;
                            }
                            if (selectAllNode.isChecked()) {
                                try {
                                    Thread.sleep(20);
                                } catch (Exception ep) {
                                    ep.printStackTrace();
                                }
                            }
                            mProgressBar.setProgress(AddrOffset+read_data_num);
                        }
                        upgrateStatus.setText("正在发送固件数据...");
                    }
                    //执行固件
                    upgrateStatus.setText("执行固件...");
                    ret = usb2xxx.usb2can.CAN_BL_Excute(0, (byte) 0, NodeAddr, USB2CAN.CAN_BL_APP);
                    if (ret != USB2CAN.CAN_SUCCESS) {
                        Toast.makeText(MainActivity.this, "执行固件跳转命令失败！", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    try {
                        Thread.sleep(500);
                    } catch (Exception ep) {
                        ep.printStackTrace();
                    }
                    //检测当前节点是否为App
                    if (!selectAllNode.isChecked()) {
                        int[] appVersion = new int[1], appType = new int[1];
                        ret = usb2xxx.usb2can.CAN_BL_NodeCheck(0, (byte) 0, NodeAddr, appVersion, appType, 10);
                        if (ret != USB2CAN.CAN_SUCCESS) {
                            Toast.makeText(MainActivity.this, "节点检测失败！", Toast.LENGTH_SHORT).show();
                            upgrateStatus.setText("节点检测失败！");
                            return;
                        } else {
                            if (appType[0] != USB2CAN.CAN_BL_APP) {
                                Toast.makeText(MainActivity.this, "固件升级失败，请重新点击升级固件！", Toast.LENGTH_SHORT).show();
                                return;
                            }else{
                                upgrateStatus.setText("固件升级成功！");
                            }
                        }
                    }
                } else {
                    Toast.makeText(MainActivity.this, "打开固件文件失败", Toast.LENGTH_SHORT).show();
                }
            }
        });

        setNewBaudRate = (Button)findViewById(R.id.buttonSetNewBaudRate);
        setNewBaudRate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                short NodeAddr;
                //初始化配置设备
                if (!configUSB2CANDevice(0, (byte) 0, Integer.parseInt(baudRate.getSelectedItem().toString().substring(0, baudRate.getSelectedItem().toString().length() - 5))*1000)) {
                    return;
                }
                //判断当前所选择的节点
                if (selectAllNode.isChecked()) {
                    NodeAddr = 0;
                } else {
                    if (nodeAddr.getText().toString().isEmpty()) {
                        //没有选择任何节点
                        Toast.makeText(MainActivity.this, "请先选择节点！", Toast.LENGTH_SHORT).show();
                        return;
                    } else {
                        NodeAddr = (short) Integer.parseInt(nodeAddr.getText().toString());
                    }
                }
                //
                USB2CAN.CAN_INIT_CONFIG CAN_InitConfig = usb2xxx.usb2can. new CAN_INIT_CONFIG();
                int newBaudRateValue = Integer.parseInt(newBaudRate.getSelectedItem().toString().substring(0, newBaudRate.getSelectedItem().toString().length() - 5))*1000;
                CANBaudRate mCANBaudRate = new CANBaudRate();
                CAN_InitConfig.CAN_BRP = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(newBaudRateValue)].PreScale;
                CAN_InitConfig.CAN_SJW = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(newBaudRateValue)].SJW;
                CAN_InitConfig.CAN_BS1 = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(newBaudRateValue)].BS1;
                CAN_InitConfig.CAN_BS2 = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(newBaudRateValue)].BS2;
                int ret = usb2xxx.usb2can.CAN_BL_SetNewBaudRate(0, (byte)0, NodeAddr, CAN_InitConfig, newBaudRateValue, 100);
                if (ret != USB2CAN.CAN_SUCCESS) {
                    Toast.makeText(MainActivity.this, "设置新波特率失败！", Toast.LENGTH_SHORT).show();
                } else {
                    SpinnerAdapter apsAdapter= baudRate.getAdapter();
                    int k= apsAdapter.getCount();
                    for(int i=0;i<k;i++){
                        if(newBaudRate.getSelectedItem().toString().equals(apsAdapter.getItem(i).toString())){
                            baudRate.setSelection(i,true);
                            break;
                        }
                    }
                    upgrateStatus.setText("设置新波特率成功！");
                }
            }
        });
    }
    private boolean configUSB2CANDevice(int DevIndex,byte CANIndex,int BaudRate) {
        int ret;
        //扫描设备连接数
        int devNum = usb2xxx.device.USB_ScanDevice();
        if (devNum <= 0) {
            Toast.makeText(MainActivity.this, "当前设备未连接", Toast.LENGTH_SHORT).show();
            return false;
        }
        //打开设备
        if (!usb2xxx.device.USB_OpenDevice(DevIndex)) {
            Toast.makeText(MainActivity.this, "打开设备失败", Toast.LENGTH_SHORT).show();
            return false;
        }
        USB2CAN.CBL_CMD_LIST CMD_List = usb2xxx.usb2can. new CBL_CMD_LIST();
        CMD_List.Erase = 0;
        CMD_List.WriteInfo = 1;
        CMD_List.Write = 2;
        CMD_List.Check = 3;
        CMD_List.SetBaudRate = 4;
        CMD_List.Excute = 5;
        CMD_List.CmdSuccess = 8;
        CMD_List.CmdFaild = 9;
        USB2CAN.CAN_INIT_CONFIG CAN_InitConfig = usb2xxx.usb2can. new CAN_INIT_CONFIG();
        CANBaudRate mCANBaudRate = new CANBaudRate();
        CAN_InitConfig.CAN_BRP = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(BaudRate)].PreScale;
        CAN_InitConfig.CAN_SJW = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(BaudRate)].SJW;
        CAN_InitConfig.CAN_BS1 = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(BaudRate)].BS1;
        CAN_InitConfig.CAN_BS2 = mCANBaudRate.CANBaudRateTab[mCANBaudRate.GetBaudRateIndex(BaudRate)].BS2;

        ret = usb2xxx.usb2can.CAN_BL_Init(DevIndex, CANIndex, CAN_InitConfig, CMD_List);
        if(ret!=USB2CAN.CAN_SUCCESS)
        {
            Toast.makeText(MainActivity.this, "初始化设备失败", Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    /**
     * Checks if the app has permission to write to device storage
     *
     * If the app does not has permission then the user will be prompted to
     * grant permissions
     *
     * @param activity
     */
    public static void verifyStoragePermissions(Activity activity) {
        // Check if we have write permission
         int permission = ActivityCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE);
         if (permission != PackageManager.PERMISSION_GRANTED) {
             // We don't have permission so prompt the user
             ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE,
             REQUEST_EXTERNAL_STORAGE);
         }
    }
    private void showFileChooser() {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);

        try {
            startActivityForResult( Intent.createChooser(intent, "Select a File to Upload"), REQUEST_GET_FIRMWARE_FILE);
        } catch (android.content.ActivityNotFoundException ex) {
            Toast.makeText(this, "Please install a File Manager.",  Toast.LENGTH_SHORT).show();
        }
    }
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (resultCode == RESULT_OK) {
            if (requestCode == REQUEST_GET_FIRMWARE_FILE) {
                Uri uri = intent.getData();
                String path = FileUtils.getPath(this, uri);
                firmwareFile.setText(path);
                upgrateStatus.setText("选择了固件文件");
                System.out.println("select: " + path);
            }
            if(requestCode == REQUEST_GET_CAN_NODE){
                String nodeAddrStr = intent.getStringExtra("nodeAddr");
                nodeAddr.setText(nodeAddrStr);
                upgrateStatus.setText("成功选择节点");
                System.out.println("nodeAddr: " + nodeAddr);
            }
        }
    }

    public static class FileUtils {
        public static String getPath(Context context, Uri uri) {
            if ("content".equalsIgnoreCase(uri.getScheme())) {
                String[] projection = { "_data" };
                Cursor cursor = null;
                try {
                    cursor = context.getContentResolver().query(uri, projection,null, null, null);
                    int column_index = cursor.getColumnIndexOrThrow("_data");
                    if (cursor.moveToFirst()) {
                        return cursor.getString(column_index);
                    }
                } catch (Exception e) {
                    // Eat it
                }
            }

            else if ("file".equalsIgnoreCase(uri.getScheme())) {
                return uri.getPath();
            }
            return null;
        }
    }
}
