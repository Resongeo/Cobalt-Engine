class Player : IEntity
{
    Entity self;

    void Start() {
        vec2 pos = vec2(10);
        Print("Hellope - " + pos.x + " " + pos.y);
    }

    void Update(float dt) {
        self.set_rotation(self.get_rotation() + 0.3);
        self.set_scale_x(self.get_scale_x() + 0.01);
    }
}