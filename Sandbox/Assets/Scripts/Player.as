class Player : IEntity
{
    Entity self;
    float time = 0.0f;

    void Update(float dt) {
        time += dt * 3;
        self.set_position_x(sin(time));
        self.set_position_y(cos(time));
        self.set_scale_x((sin(time) * 0.5) + 1);
        self.set_rotation((time * 10) % 360);
    }
}