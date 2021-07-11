## Proposal

* Project Title: N-body solvers
* Your Name: Di Wu
* Team Members: Me, Jasper, Jingyu

## Project Description (Reference Pacheco P.271)

The N-body problem aims to find the positions and the velocities of a collection of particles over a period of time. We want to simulate the motion of the planets and the stars. The **input** of the program includes

- ``n``: number of particles
- ``delta_t``, ``T``: unit time interval and interval
- ``mass`` for each particle
- ``initial position`` for each particle
- ``initial velocity`` for each particle.

We will use Newton's second and third law of motion and the law of universal graviation to determine the positions and velocities of stars for each interval ``delta_t``. For each time interval, we have to re-calculate the total force on each star ``q`` and get its accerlation, which is the second derivative of ``s_q``. Hence we have a system of differential equations. The **output** of the problem is the position and the velocities of starts at time 0, delta_t, 2*delta_t, ..., T*delta_t.

## Score Sheet

| Item                    | Max Value |    Expected Deadline | Achieved Date |
|-------------------------|----------:|---------------------:|---------------|
| Serial Implementation   |        20 |         May 30, 2021 | May 29, 2021  |
| Parallel Implementation |        40 |         June 5, 2021 | June 5, 2021  |
| Correctness             |        20 |         June 5, 2021 | June 5, 2021  |
| Writeup/Analysis        |        20 |         June 8, 2021 |               |
| Wattage                 |        +5 |         June 8, 2021 |               |
| Utility                 |       +15 |        June 10, 2021 |               |
| Sub-Total               |      /120 |                      |               |
| Challenge Modifier      |         x | (John will fill out) |               |
| Group Discount          |     0.8*N | (John will fill out) |               |
| Total                   |           |                      |               |