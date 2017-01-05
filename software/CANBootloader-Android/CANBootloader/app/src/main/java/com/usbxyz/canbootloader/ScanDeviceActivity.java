package com.usbxyz.canbootloader;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.usbxyz.usb2xxx.USB2CAN;
import com.usbxyz.usb2xxx.USB2XXX;

import java.util.ArrayList;

public class ScanDeviceActivity extends AppCompatActivity {
    USB2XXX usb2xxx;
    boolean mScanning = false;
    private CANDeviceListAdapter mCANDeviceListAdapter = null;
    private ListView mListView;
    int startAddr,endAddr,baudRate;
    Thread threadScanNode = null;
    public class ConnectStateChanged implements USB2XXX.DeviceConnectStateChanged{
        @Override
        public void stateChanged(boolean connected) {
            if(connected){
                Toast.makeText(ScanDeviceActivity.this, "Device Attached", Toast.LENGTH_SHORT).show();
            }else{
                Toast.makeText(ScanDeviceActivity.this, "Device Detached", Toast.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scan_device);
        // Initializes list view adapter.
        mCANDeviceListAdapter = new CANDeviceListAdapter(this);
        mListView = (ListView) findViewById(R.id.devicelist);
        mListView.setAdapter(mCANDeviceListAdapter);
        mListView.setOnItemClickListener(mDeviceScanListener);
        //用于测试
        //mCANDeviceListAdapter.addDevice(new CANNodeDevice((short)0x1234,0x00010001,USB2CAN.CAN_BL_BOOT));
        //mCANDeviceListAdapter.addDevice(new CANNodeDevice((short)0x5678,0x00020001,USB2CAN.CAN_BL_APP));

        //新页面接收数据
        Bundle bundle = this.getIntent().getExtras();
        //接收name值
        startAddr = Integer.parseInt(bundle.getString("startAddr"));
        endAddr = Integer.parseInt(bundle.getString("endAddr"));
        baudRate = Integer.parseInt(bundle.getString("baudRate"))*1000;
        System.out.println("startAddr = "+startAddr);
        System.out.println("endAddr = "+endAddr);
        System.out.println("baudRate = "+baudRate);
        usb2xxx = new USB2XXX(this,new ConnectStateChanged());//需要监视设备插拔事件
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        if (!mScanning) {
            menu.findItem(R.id.menu_stop).setVisible(false);
            menu.findItem(R.id.menu_scan).setVisible(true);
            menu.findItem(R.id.menu_refresh).setActionView(null);
        } else {
            menu.findItem(R.id.menu_stop).setVisible(true);
            menu.findItem(R.id.menu_scan).setVisible(false);
            menu.findItem(R.id.menu_refresh).setActionView(R.layout.actionbar_indeterminate_progress);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_scan:
                scanCANNodeDevice(true);
                break;
            case R.id.menu_stop:
                scanCANNodeDevice(false);
                break;
        }
        return true;
    }

    // Adapter for holding devices found through scanning.
    private class CANDeviceListAdapter extends BaseAdapter {
        private ArrayList<CANNodeDevice> mCANNodeDevices;
        private LayoutInflater mInflator;

        public CANDeviceListAdapter(Context context) {
            super();
            this.mInflator = LayoutInflater.from(context);
            mCANNodeDevices = new ArrayList<CANNodeDevice>();
        }

        public void addDevice(CANNodeDevice device) {
            if (!mCANNodeDevices.contains(device)) {
                mCANNodeDevices.add(device);
            }
        }

        public CANNodeDevice getDevice(int position) {
            return mCANNodeDevices.get(position);
        }

        public void refreshDevice(int position) {
            if (position > 0) {
                CANNodeDevice device = mCANNodeDevices.get(position);
                for (int i = position; i > 0; i--) {
                    CANNodeDevice idevice = mCANNodeDevices.get(i - 1);
                    mCANNodeDevices.set(i, idevice);
                }
                mCANNodeDevices.set(0, device);
            }
        }

        public void clear() {
            mCANNodeDevices.clear();
        }

        @Override
        public int getCount() {
            return mCANNodeDevices.size();
        }

        @Override
        public Object getItem(int i) {
            return mCANNodeDevices.get(i);
        }

        @Override
        public long getItemId(int i) {
            return i;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            ViewHolder viewHolder;
            // General ListView optimization code.
            if (view == null) {
                view = mInflator.inflate(R.layout.can_node_item, null);
                viewHolder = new ViewHolder();
                viewHolder.nodeAddr = (TextView) view.findViewById(R.id.node_address);
                viewHolder.firmwareType = (TextView) view.findViewById(R.id.firmware_type);
                viewHolder.firmwareVersion = (TextView) view.findViewById(R.id.firmware_version);
                view.setTag(viewHolder);
            } else {
                viewHolder = (ViewHolder) view.getTag();
            }
            CANNodeDevice device = mCANNodeDevices.get(i);
            final String deviceName = String.format("0x%04X",device.NodeAddr);
            if (deviceName != null && deviceName.length() > 0) {
                viewHolder.nodeAddr.setText(deviceName);
            }
            String AppTypeStr;
            if (device.Type == USB2CAN.CAN_BL_BOOT) {
                AppTypeStr = "BOOT";
            } else if (device.Type == USB2CAN.CAN_BL_APP) {
                AppTypeStr = "APP";
            }else{
                AppTypeStr = "UNKNOWN";
            }
            viewHolder.firmwareType.setText(AppTypeStr);
            String AppVersionStr = String.format("v%d.%d", (((device.Version >> 24) & 0xFF) * 10) + (device.Version >> 16) & 0xFF, (((device.Version >> 8) & 0xFF) * 10) + device.Version & 0xFF);
            viewHolder.firmwareVersion.setText(AppVersionStr);
            return view;
        }
    }

    AdapterView.OnItemClickListener mDeviceScanListener = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position,
                                long id) {
            if (mScanning) {
                scanCANNodeDevice(false);
            }

            mCANDeviceListAdapter.refreshDevice(position);
            mCANDeviceListAdapter.notifyDataSetChanged();
            // TODO Auto-generated method stub
            final CANNodeDevice device = mCANDeviceListAdapter.getDevice(0);
            if (device == null) {
                return;
            }else{
                Intent intent = new Intent();
                intent.putExtra("nodeAddr", String.format("%d",device.NodeAddr));// 放入返回值
                setResult(RESULT_OK, intent);// 放入回传的值,并添加一个Code,方便区分返回的数据
                finish();// 结束当前的activity,等于点击返回按钮
            }
        }
    };

    private boolean configUSB2CANDevice(int DevIndex,byte CANIndex,int BaudRate) {
        int ret;
        //扫描设备连接数
        int devNum = usb2xxx.device.USB_ScanDevice();
        if (devNum <= 0) {
            Toast.makeText(ScanDeviceActivity.this, "当前设备未连接", Toast.LENGTH_SHORT).show();
            return false;
        }
        //打开设备
        if (!usb2xxx.device.USB_OpenDevice(DevIndex)) {
            Toast.makeText(ScanDeviceActivity.this, "打开设备失败", Toast.LENGTH_SHORT).show();
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
            Toast.makeText(ScanDeviceActivity.this, "初始化设备失败", Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    private void scanCANNodeDevice(final boolean enable) {
        if (enable) {
            if(configUSB2CANDevice(0,(byte)0,baudRate)) {
                mScanning = true;
                threadScanNode = new Thread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        for (int i = startAddr; (i <= endAddr)&&mScanning; i++)
                        {
                            int ret;
                            int[] appVersion = new int[1], appType = new int[1];
                            ret = usb2xxx.usb2can.CAN_BL_NodeCheck(0, (byte) 0, (short) i, appVersion, appType, 10);
                            if (ret == USB2CAN.CAN_SUCCESS)
                            {
                                final CANNodeDevice nodeDevice = new CANNodeDevice((short)i,appVersion[0],appType[0]);
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        mCANDeviceListAdapter.addDevice(nodeDevice);
                                        mCANDeviceListAdapter.notifyDataSetChanged();
                                    }
                                });
                            }
                        }
                        mScanning = false;
                        invalidateOptionsMenu();
                    }
                });
                threadScanNode.start();
            }
        } else {
            mScanning = false;
        }
        invalidateOptionsMenu();
    }

    private class CANNodeDevice{
        short NodeAddr;
        int Version;
        int Type;

        CANNodeDevice(short NodeAddr,int Version,int Type)
        {
            this.NodeAddr = NodeAddr;
            this.Version = Version;
            this.Type = Type;
        }
    }

    static class ViewHolder {
        TextView nodeAddr;
        TextView firmwareVersion;
        TextView firmwareType;
    }
}
