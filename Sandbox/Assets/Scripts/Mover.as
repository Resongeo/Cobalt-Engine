class Mover : IEntity
{
    Entity self;

    float ground_start_pos_y = 0;
    float time = 0;

    void Start() {
        ground_start_pos_y = self.get_position_y();
    }

    void Update(float dt) {
        time += dt * 5;
        self.set_position_y(ground_start_pos_y + sin(time) * 0.2);
    }
}