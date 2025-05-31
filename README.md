Answers task 1:

# توضیح الگوریتم‌های زمان‌بندی پردازنده (Process Scheduling)

## تعریف معیار های زمانی:

### 1. **Turnaround Time (زمان چرخش)**
زمان کل از لحظه ورود پروسه تا اتمام آن
```
Turnaround Time = Completion Time - Arrival Time
```

### 2. **Waiting Time (زمان انتظار)**
زمانی که پروسه در صف منتظر اجرا بوده است
```
Waiting Time = Turnaround Time - Burst Time
```

### 3. **Response Time (زمان پاسخ)**
زمان از ورود پروسه تا اولین بار اجرا شدن
```
Response Time = Start Time - Arrival Time
```

---

## الگوریتم اول: FCFS (First Come First Served)

### نحوه کارکرد:
- **اولین رسیده، اولین سرویس**: پروسه‌ها به ترتیب زمان ورودشان اجرا می‌شوند
- **غیرقابل وقفه (Non-preemptive)**: پروسه تا پایان اجرا می‌شود
- **مرتب‌سازی**: ابتدا بر اساس arrival time، سپس PID

### محاسبه زمان‌ها در FCFS:
```c
// زمان شروع = زمان فعلی سیستم
startTime = current_time;

// زمان اتمام = زمان شروع + مدت اجرا
completionTime = startTime + burstTime;

// زمان چرخش
turnaround_time = completionTime - arrivalTime;

// زمان انتظار = زمان شروع - زمان ورود
waiting_time = startTime - arrivalTime;

// زمان پاسخ = زمان انتظار (چون غیرقابل وقفه است)
response_time = startTime - arrivalTime;
```

### ویژگی‌های کلیدی:
- CPU در صورت بیکاری منتظر می‌ماند تا پروسه بعدی برسد
- ساده‌ترین الگوریتم اما ممکن است کارآمد نباشد

---

## الگوریتم دوم: SJF (Shortest Job First)

### نحوه کارکرد:
- **کوتاه‌ترین کار اول**: در هر لحظه، پروسه با کمترین burst time انتخاب می‌شود
- **غیرقابل وقفه**: پروسه تا پایان ادامه می‌یابد
- **انتخاب هوشمند**: از بین پروسه‌های رسیده، بهترین را انتخاب می‌کند

### الگوریتم انتخاب:
```c
while (completed_count < n) {
    // پیدا کردن کوتاه‌ترین کار از پروسه‌های رسیده
    for (int i = 0; i < n; i++) {
        if (!is_completed[i] && arrivalTime <= current_time) {
            if (burstTime < shortest_burst) {
                // پروسه جدید کوتاه‌تر است
            } else if (burstTime == shortest_burst) {
                // تساوی: ابتدا بر اساس arrival time، سپس PID
            }
        }
    }
}
```

### محاسبه زمان‌ها در SJF:
```c
// مشابه FCFS اما انتخاب پروسه متفاوت است
startTime = current_time;
completionTime = startTime + burstTime;
turnaround_time = completionTime - arrivalTime;
waiting_time = startTime - arrivalTime;  // غیرقابل وقفه
response_time = startTime - arrivalTime;
```

### مدیریت بیکاری CPU:
- اگر هیچ پروسه‌ای آماده نباشد، CPU به زمان ورود پروسه بعدی می‌پرد

---

## الگوریتم سوم: Round Robin (RR)

### نحوه کارکرد:
- **زمان‌بندی دایره‌ای**: هر پروسه حداکثر یک time quantum اجرا می‌شود
- **قابل وقفه (Preemptive)**: پروسه‌ها می‌توانند متوقف و دوباره شروع شوند
- **صف آماده**: پروسه‌ها در یک صف دایره‌ای قرار می‌گیرند

### مدیریت صف آماده:
```c
// اضافه کردن پروسه‌های رسیده به صف
for (int i = 0; i < n; i++) {
    if (remainingTime > 0 && arrivalTime <= current_time && !in_queue) {
        ready_queue[q_rear] = i;
        in_queue_flags[i] = true;
    }
}
```

### منطق اجرا:
```c
if (remainingTime <= timeQuantum) {
    // پروسه تمام می‌شود
    current_time += remainingTime;
    remainingTime = 0;
    completionTime = current_time;
} else {
    // پروسه وقفه می‌شود
    current_time += timeQuantum;
    remainingTime -= timeQuantum;
    // دوباره به انتهای صف اضافه می‌شود
}
```

### محاسبه زمان‌ها در Round Robin:
```c
// زمان چرخش
turnaround = completionTime - arrivalTime;

// زمان انتظار
waiting = turnaround - burstTime;  // کل زمان منهای زمان اجرای واقعی

// زمان پاسخ
response = startTime - arrivalTime;  // اولین بار اجرا شدن
```

### ویژگی‌های خاص RR:
- **startTime** تنها یک بار در اولین اجرا تنظیم می‌شود
- **remainingTime** برای پیگیری زمان باقی‌مانده استفاده می‌شود
- **صف دایره‌ای** برای مدیریت عادلانه پروسه‌ها

---
هر سه الگوریتم میانگین این زمان‌ها را محاسبه کرده و در ساختار `Metrics` برمی‌گردانند.


مورد آزمایش 1: سه پروسه متوالی
ورودی:

P1: arrival=0, burst=10
P2: arrival=1, burst=5
P3: arrival=2, burst=8
Time Quantum = 4

نتایج:

FCFS & SJF یکسان: زیرا پروسه‌ها به ترتیب زمان ورود دارای کوتاه‌ترین کار نیستند

Turnaround: 15.00, Waiting: 7.33, Response: 7.33


Round Robin بهتر در Response: 3.00 (سریع‌تر پاسخ می‌دهد)

اما Turnaround بالاتر: 19.33 (به دلیل context switching)



مورد آزمایش 2: پروسه‌ها با فاصله زمانی
ورودی:

P1: arrival=2, burst=6
P2: arrival=5, burst=2
P3: arrival=8, burst=4
Time Quantum = 2

نتایج:

FCFS & SJF مشابه: 5.67, 1.67, 1.67
RR کمی بهتر در Response: 1.00 (پاسخ سریع‌تر)
دلیل شباهت: فاصله زمانی بین پروسه‌ها زیاد است

مورد آزمایش 3: چهار پروسه با burst های متفاوت
ورودی:

P1: arrival=0, burst=4
P2: arrival=1, burst=3
P3: arrival=2, burst=1 (کوتاه‌ترین)
P4: arrival=3, burst=2
Time Quantum = 2

نتایج:

SJF بهترین عملکرد: 5.00, 2.50, 2.50 (P3 را زودتر اجرا می‌کند)
FCFS متوسط: 5.75, 3.25, 3.25
RR بهترین Response: 1.75 اما Turnaround بالاتر: 6.25

مورد آزمایش 4: پنج پروسه پیچیده
ورودی:

P1-P5 با arrival های 0,2,4,6,8 و burst های 3,6,4,5,2
Time Quantum = 3

نتایج:

SJF بهترین کل عملکرد: 7.60, 3.60, 3.60
FCFS: 8.60, 4.60, 4.60
RR: Response عالی (2.60) اما Turnaround بالا (10.60)

مورد آزمایش 5: تک پروسه (حالت ویژه)
ورودی:

P1: arrival=0, burst=5
Time Quantum = 2

نتایج:

همه الگوریتم‌ها یکسان: 5.00, 0.00, 0.00
طبیعی است زیرا تنها یک پروسه وجود دارد

مورد آزمایش 6: دو پروسه همزمان
ورودی:

P1: arrival=0, burst=4
P2: arrival=0, burst=6
Time Quantum = 3

نتایج:

FCFS & SJF: 7.00, 2.00, 2.00 (P1 کوچک‌تر اول اجرا می‌شود)
RR: Response بهتر (1.50) اما Turnaround بالاتر (8.50)

مورد آزمایش 7: چهار پروسه با ورود متنوع
ورودی:

P1-P4 با burst های 8,4,9,5
Time Quantum = 3

نتایج:

SJF برتر: 14.25, 7.75, 7.75
FCFS: 15.25, 8.75, 8.75
RR: Response فوق‌العاده (3.00) اما Turnaround بالا (20.00)

مورد آزمایش 8: شش پروسه پیچیده
ورودی:

شش پروسه با الگوی پیچیده
Time Quantum = 4

نتایج:

SJF بهترین: 15.17, 9.33, 9.33
FCFS: 17.00, 11.17, 11.17
RR: Response عالی (6.33) اما Turnaround بالا (20.67)


نتیجه‌گیری کلی:
الگوی مشاهده شده:

SJF معمولاً بهترین Turnaround و Waiting Time را دارد
Round Robin همیشه بهترین Response Time را ارائه می‌دهد
FCFS ساده اما اغلب متوسط عمل می‌کند
Trade-off مهم: RR پاسخ سریع اما overhead بیشتر دارد

کاربرد عملی:

سیستم‌های تعاملی: Round Robin (پاسخ سریع)
سیستم‌های batch: SJF (کارایی بالا)
سیستم‌های ساده: FCFS (پیاده‌سازی آسان)

output: 

==== Test Case 1 ====
FCFS: Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
SJF:  Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
RR (Quantum = 4): Calculated: Turnaround: 19.33, Waiting: 11.67, Response: 3.00
         Expected:   Turnaround: 19.33, Waiting: 11.67, Response: 3.00
>>> Test Case 1 PASSED.

==== Test Case 2 ====
FCFS: Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.67
      Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.67
SJF:  Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.67
      Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.67
RR (Quantum = 2): Calculated: Turnaround: 5.67, Waiting: 1.67, Response: 1.00
         Expected:   Turnaround: 5.67, Waiting: 1.67, Response: 1.00
>>> Test Case 2 PASSED.

==== Test Case 3 ====
FCFS: Calculated: Turnaround: 5.75, Waiting: 3.25, Response: 3.25
      Expected:   Turnaround: 5.75, Waiting: 3.25, Response: 3.25
SJF:  Calculated: Turnaround: 5.00, Waiting: 2.50, Response: 2.50
      Expected:   Turnaround: 5.00, Waiting: 2.50, Response: 2.50
RR (Quantum = 2): Calculated: Turnaround: 6.25, Waiting: 3.75, Response: 1.75
         Expected:   Turnaround: 6.25, Waiting: 3.75, Response: 1.75
>>> Test Case 3 PASSED.

==== Test Case 4 ====
FCFS: Calculated: Turnaround: 8.60, Waiting: 4.60, Response: 4.60
      Expected:   Turnaround: 8.60, Waiting: 4.60, Response: 4.60
SJF:  Calculated: Turnaround: 7.60, Waiting: 3.60, Response: 3.60
      Expected:   Turnaround: 7.60, Waiting: 3.60, Response: 3.60
RR (Quantum = 3): Calculated: Turnaround: 10.60, Waiting: 6.60, Response: 2.60
         Expected:   Turnaround: 10.60, Waiting: 6.60, Response: 2.60
>>> Test Case 4 PASSED.

==== Test Case 5 (Single Process) ====
FCFS: Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
      Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
SJF:  Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
      Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
RR (Quantum = 2): Calculated: Turnaround: 5.00, Waiting: 0.00, Response: 0.00
         Expected:   Turnaround: 5.00, Waiting: 0.00, Response: 0.00
>>> Test Case 5 (Single Process) PASSED.

==== Test Case 6 (Two Processes, Same Arrival) ====
FCFS: Calculated: Turnaround: 7.00, Waiting: 2.00, Response: 2.00
      Expected:   Turnaround: 7.00, Waiting: 2.00, Response: 2.00
SJF:  Calculated: Turnaround: 7.00, Waiting: 2.00, Response: 2.00
      Expected:   Turnaround: 7.00, Waiting: 2.00, Response: 2.00
RR (Quantum = 3): Calculated: Turnaround: 8.50, Waiting: 3.50, Response: 1.50
         Expected:   Turnaround: 8.50, Waiting: 3.50, Response: 1.50
>>> Test Case 6 (Two Processes, Same Arrival) PASSED.

==== Test Case 7 (Varying Arrivals) ====
FCFS: Calculated: Turnaround: 15.25, Waiting: 8.75, Response: 8.75
      Expected:   Turnaround: 15.25, Waiting: 8.75, Response: 8.75
SJF:  Calculated: Turnaround: 14.25, Waiting: 7.75, Response: 7.75
      Expected:   Turnaround: 14.25, Waiting: 7.75, Response: 7.75
RR (Quantum = 3): Calculated: Turnaround: 20.00, Waiting: 13.50, Response: 3.00
         Expected:   Turnaround: 20.00, Waiting: 13.50, Response: 3.00
>>> Test Case 7 (Varying Arrivals) PASSED.

==== Test Case 8 (Six Processes) ====
FCFS: Calculated: Turnaround: 17.00, Waiting: 11.17, Response: 11.17
      Expected:   Turnaround: 17.00, Waiting: 11.17, Response: 11.17
SJF:  Calculated: Turnaround: 15.17, Waiting: 9.33, Response: 9.33
      Expected:   Turnaround: 15.17, Waiting: 9.33, Response: 9.33
RR (Quantum = 4): Calculated: Turnaround: 20.67, Waiting: 14.83, Response: 6.33
         Expected:   Turnaround: 20.67, Waiting: 14.83, Response: 6.33
>>> Test Case 8 (Six Processes) PASSED.

ALL TESTS PASSED.


/////
# CPU Scheduling Algorithms

This project implements three fundamental CPU scheduling algorithms in C as part of an Operating Systems course project.

## Algorithms Implemented

1. **FCFS (First Come First Serve)**
2. **SJF (Shortest Job First - Non-Preemptive)**
3. **Round Robin (RR)**

Each algorithm computes the following performance metrics:
- Average Turnaround Time
- Average Waiting Time
- Average Response Time

## File Structure

```
.
├── scheduler.h         # Declarations for structs and scheduling functions
├── scheduler.c         # Implementation of the scheduling algorithms
├── scheduler_test.c    # Test cases and assertions for validating correctness
├── Makefile            # Build automation script
└── README.md           # Project documentation (this file)
```

## How to Compile

To build the project, simply use:

```bash
make
```

This compiles the source files and produces an executable named `scheduler_test`.

## How to Run

To run the compiled test program:

```bash
make run
```

This will execute the scheduler tests and print the calculated vs. expected metrics for:
- FCFS
- SJF
- Round Robin (with specified quantum)

If all assertions pass, you will see:

```
>>> Test Case X PASSED.
...
ALL TESTS PASSED.
```

## How to Clean

To remove the compiled binary:

```bash
make clean
```

## Example Output (Truncated)

```
==== Test Case 1 ====
FCFS: Calculated: Turnaround: 15.00, Waiting: 7.33, Response: 7.33
      Expected:   Turnaround: 15.00, Waiting: 7.33, Response: 7.33
SJF:  ...
RR (Quantum = 4): ...
>>> Test Case 1 PASSED.
```

## Notes

- The `Process` struct contains fields for tracking all timing metrics.
- All algorithms are non-preemptive except RR which uses time slicing.
- The project is written in standard C and should work on any Unix-like system (Linux, macOS, WSL).
