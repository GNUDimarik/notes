package com.packtpub;

import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.primitives.Booleans;
import com.google.common.primitives.Bytes;
import com.google.common.primitives.Chars;
import com.google.common.primitives.Doubles;
import com.google.common.primitives.Floats;
import com.google.common.primitives.Ints;
import com.google.common.primitives.Longs;
import com.google.common.primitives.Shorts;
import com.packtpub.exception.InvalidTypeException;
import com.packtpub.exception.NotExistingKeyException;
import com.packtpub.exception.StoreFullException;

/**
 * Main application activity which allows getting and setting data in
 * a key/value store implemented on the native side.
 */
public class StoreActivity extends Activity {
    // UI components
    private EditText mUIKeyEdit;
    private EditText mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton;
    private Button mUISetButton;

    private Pattern mKeyPattern;

    // Members.
    private Store mStore;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // Initializes text components.
        mKeyPattern = Pattern.compile("\\p{Alnum}+");
        mUIKeyEdit = (EditText) findViewById(R.id.uiKeyEdit);
        mUIValueEdit = (EditText) findViewById(R.id.uiValueEdit);

        // Initializes type selector.
        ArrayAdapter<StoreType> lAdapter = new ArrayAdapter<StoreType>(
            this, android.R.layout.simple_spinner_item,
            StoreType.values());
        lAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mUITypeSpinner = (Spinner) findViewById(R.id.uiTypeSpinner);
        mUITypeSpinner.setAdapter(lAdapter);

        // Initializes buttons.
        mUIGetButton = (Button) findViewById(R.id.uiGetValueButton);
        mUIGetButton.setOnClickListener(new OnClickListener() {
            public void onClick(View pView) {
                onGetValue();
            }
        });
        mUISetButton = (Button) findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new OnClickListener() {
            public void onClick(View pView) {
                onSetValue();
            }
        });

        // Initializes the native side store and recreates it if it
        // has already been created by a previous activity instance.
        mStore = new Store();
        mStore.initializeStore();
    }

    @Override
    protected void onDestroy() {
        // Releases the native side store to avoid memory leaks if a
        // new activity instance is launched (native-side module may
        // not be unloaded meanwhile but kept in memory).
        super.onDestroy();
        mStore.finalizeStore();
    }

    private void onGetValue() {
        // Retrieves key and type entered by the user.
        String lKey = mUIKeyEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                        .getSelectedItem();

        // Checks key is correct.
        if (!mKeyPattern.matcher(lKey).matches()) {
            displayError("Incorrect key: only alphanumeric characters are allowed.");
            return;
        }

        // Retrieves value from the store and displays it.
        // Each data type has its own access method.
        try {
            switch (lType) {
            /**
             * Primitives and objects.
             */
            case Boolean:
                mUIValueEdit.setText(Boolean.toString(mStore
                                .getBoolean(lKey)));
                break;
            case Byte:
                mUIValueEdit.setText(Byte.toString(mStore
                                .getByte(lKey)));
                break;
            case Char:
                mUIValueEdit.setText(Character.toString(mStore
                                .getChar(lKey)));
                break;
            case Double:
                mUIValueEdit.setText(Double.toString(mStore
                                .getDouble(lKey)));
                break;
            case Float:
                mUIValueEdit.setText(Float.toString(mStore
                                .getFloat(lKey)));
                break;
            case Integer:
                mUIValueEdit.setText(Integer.toString(mStore
                                .getInteger(lKey)));
                break;
            case Long:
                mUIValueEdit.setText(Long.toString(mStore
                                .getLong(lKey)));
                break;
            case Short:
                mUIValueEdit.setText(Short.toString(mStore
                                .getShort(lKey)));
                break;
            case String:
                mUIValueEdit.setText(mStore.getString(lKey));
                break;
            case Color:
                mUIValueEdit.setText(mStore.getColor(lKey)
                                .toString());
                break;

            /**
             * Arrays.
             */
            case BooleanArray:
                mUIValueEdit.setText(Booleans.join(";",
                                mStore.getBooleanArray(lKey)));
                break;
            // @formatter:off
            case ByteArray:
                mUIValueEdit.setText(Joiner.on(";").join(
                    Bytes.asList(mStore.getByteArray(lKey))));
                break;
            // @formatter:on
            case CharArray:
                mUIValueEdit.setText(Chars.join(";",
                                mStore.getCharArray(lKey)));
                break;
            case DoubleArray:
                mUIValueEdit.setText(Doubles.join(";",
                                mStore.getDoubleArray(lKey)));
                break;
            case FloatArray:
                mUIValueEdit.setText(Floats.join(";",
                                mStore.getFloatArray(lKey)));
                break;
            case IntegerArray:
                mUIValueEdit.setText(Ints.join(";",
                                mStore.getIntegerArray(lKey)));
                break;
            case LongArray:
                mUIValueEdit.setText(Longs.join(";",
                                mStore.getLongArray(lKey)));
                break;
            case ShortArray:
                mUIValueEdit.setText(Shorts.join(";",
                                mStore.getShortArray(lKey)));
                break;
            case StringArray:
                mUIValueEdit.setText(Joiner.on(";").join(
                                mStore.getStringArray(lKey)));
                break;
            case ColorArray:
                mUIValueEdit.setText(Joiner.on(";").join(
                                mStore.getColorArray(lKey)));
                break;
            }
        }
        // Process any exception raised while retrieving store data.
        catch (NotExistingKeyException eNotExistingKeyException) {
            displayError("Key does not exist in store");
        } catch (InvalidTypeException eInvalidTypeException) {
            displayError("Incorrect type.");
        }
    }

    private void onSetValue() {
        // Retrieves key and type entered by the user.
        String lKey = mUIKeyEdit.getText().toString();
        String lValue = mUIValueEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                        .getSelectedItem();

        // Checks key is correct.
        if (!mKeyPattern.matcher(lKey).matches()) {
            displayError("Incorrect key: only alphanumeric characters are allowed.");
            return;
        }

        // Parses user entered value and saves it in the store.
        // Each data type has its own access method.
        try {
            switch (lType) {
            /**
             * Primitives and objects.
             */
            case Boolean:
                if (lValue.equals("true") || lValue.equals("1")) {
                    mStore.setBoolean(lKey, true);
                } else if (lValue.equals("false")
                                || lValue.equals("0")) {
                    mStore.setBoolean(lKey, false);
                } else {
                    throw new IllegalArgumentException();
                }
                break;
            case Byte:
                mStore.setByte(lKey, Byte.parseByte(lValue));
                break;
            case Char:
                if (lValue.length() == 1) {
                    mStore.setChar(lKey, lValue.charAt(0));
                } else {
                    throw new IllegalArgumentException();
                }
                break;
            case Double:
                mStore.setDouble(lKey, Double.parseDouble(lValue));
                break;
            case Float:
                mStore.setFloat(lKey, Float.parseFloat(lValue));
                break;
            case Integer:
                mStore.setInteger(lKey, Integer.parseInt(lValue));
                break;
            case Long:
                mStore.setLong(lKey, Long.parseLong(lValue));
                break;
            case Short:
                mStore.setShort(lKey, Short.parseShort(lValue));
                break;
            case String:
                mStore.setString(lKey, lValue);
                break;
            case Color:
                mStore.setColor(lKey, new Color(lValue));
                break;

            /**
             * Arrays (processed with the help of Google Guava).
             */
            case BooleanArray:
                mStore.setBooleanArray(lKey, Booleans.toArray(stringToList(
                    new Function<String, Boolean>() {
                        public Boolean apply(String pSubValue) {
                            if (pSubValue.equals("true")
                                         || pSubValue.equals("1")) {
                                return Boolean.TRUE;
                            } else if (pSubValue.equals("false")
                                         || pSubValue.equals("0")) {
                                return Boolean.FALSE;
                            } else {
                                throw new IllegalArgumentException();
                            }
                        }
                    }, lValue)));
                break;

            case ByteArray:
                mStore.setByteArray(lKey, Bytes.toArray(stringToList(
                    new Function<String, Byte>() {
                        public Byte apply(String pSubValue) {
                            return Byte.parseByte(pSubValue);
                        }
                    }, lValue)));
                break;

            case CharArray:
                mStore.setCharArray(lKey, Chars.toArray(stringToList(
                    new Function<String, Character>() {
                        public Character apply(String pSubValue) {
                            if (pSubValue.length() == 1) {
                                return pSubValue.charAt(0);
                            } else {
                                throw new IllegalArgumentException();
                            }
                        }
                    }, lValue)));
                break;

            case DoubleArray:
                mStore.setDoubleArray(lKey, Doubles.toArray(stringToList(
                    new Function<String, Double>() {
                        public Double apply(String pSubValue) {
                            return Double.parseDouble(pSubValue);
                        }
                    }, lValue)));
                break;

            case FloatArray:
                mStore.setFloatArray(lKey, Floats.toArray(stringToList(
                    new Function<String, Float>() {
                        public Float apply(String pSubValue) {
                            return Float.parseFloat(pSubValue);
                        }
                    }, lValue)));
                break;
            
            case IntegerArray:
                mStore.setIntegerArray(lKey, Ints.toArray(stringToList(
                    new Function<String, Integer>() {
                        public Integer apply(String pSubValue) {
                            return Integer.parseInt(pSubValue);
                        }
                    }, lValue)));
                break;
            
            case LongArray:
                mStore.setLongArray(lKey, Longs.toArray(stringToList(
                    new Function<String, Long>() {
                        public Long apply(String pSubValue) {
                            return Long.parseLong(pSubValue);
                        }
                    }, lValue)));
                break;
            
            case ShortArray:
                mStore.setShortArray(lKey, Shorts.toArray(stringToList(
                    new Function<String, Short>() {
                        public Short apply(String pSubValue) {
                            return Short.parseShort(pSubValue);
                        }
                    }, lValue)));
                break;
            
            case StringArray:
                String[] lStringArray = lValue.split(";");
                mStore.setStringArray(lKey, lStringArray);
                break;

            case ColorArray:
                List<Color> lIdList = stringToList(
                    new Function<String, Color>() {
                        public Color apply(String pSubValue) {
                            return new Color(pSubValue);
                        }
                    }, lValue);
                Color[] lIdArray = lIdList.toArray(new Color[lIdList.size()]);
                mStore.setColorArray(lKey, lIdArray);
                break;
            }
        }
        // Process any exception raised while parsing data.
        catch (NumberFormatException eNumberFormatException) {
            displayError("Incorrect value.");
        } catch (IllegalArgumentException eIllegalArgumentException) {
            displayError("Incorrect value.");
        } catch (StoreFullException eStoreFullException) {
            displayError("Store is full.");
        }
    }

    // Helper method to display an error message.
    private void displayError(String pError) {
        Toast.makeText(getApplicationContext(), pError,
                        Toast.LENGTH_LONG).show();
    }

    // Helper method to convert a string to a list of values.
    private <TType> List<TType> stringToList(
                    Function<String, TType> pConversion,
                    String pValue) {
        String[] lSplitArray = pValue.split(";");
        List<String> lSplitList = Arrays.asList(lSplitArray);
        return Lists.transform(lSplitList, pConversion);
    }
}