package cityfreqs.com.oscoboe;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.ToggleButton;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "OSCOBOE";

    // 0hz - 22000hz: MIN 0, MAX = 22000
    private static final int FREQ_STEP = 100; // hertz
    private static final int DEFAULT_FREQ = 37; //3700
    private static final int SEEKBAR_RANGE = 220; // convert seekbar numbers to hertz range 44

    private TextView frequencyText;
    private double userFrequency;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setUpUserSynthControls();

        // initialize native audio system
        PlaybackEngine.create(this);

    }

    private void setUpUserSynthControls() {
        ToggleButton userSynthButton = findViewById(R.id.userSynthButton);
        userSynthButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    //
                    PlaybackEngine.setToneOn(true);
                }
                else {
                    //
                    PlaybackEngine.setToneOn(false);
                }
            }
        });

        userFrequency = DEFAULT_FREQ * FREQ_STEP;
        frequencyText = findViewById(R.id.frequencyText);

        SeekBar frequencySeekBar = findViewById(R.id.frequencySeekBar);
        frequencySeekBar.setMax(SEEKBAR_RANGE);
        frequencySeekBar.setProgress(DEFAULT_FREQ);
        frequencySeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                userFrequency = progress * FREQ_STEP;
                frequencyText.setText("freq: " + String.valueOf(userFrequency));
                PlaybackEngine.setFrequency(userFrequency);
                //Log.w(TAG, "freq: " + userFrequency);
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                //
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                //
            }
        });

        ToggleButton driftTestButton = findViewById(R.id.driftTestButton);
        driftTestButton.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    //
                    PlaybackEngine.runDriftTest(true);
                }
                else {
                    //
                    PlaybackEngine.runDriftTest(false);
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        PlaybackEngine.delete();
        super.onDestroy();
    }
}
